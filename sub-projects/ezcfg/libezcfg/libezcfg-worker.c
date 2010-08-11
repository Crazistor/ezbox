/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : libezcfg-worker.c
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2010 by ezbox-project
 *
 * History      Rev       Description
 * 2010-07-12   0.1       Write it from scratch
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stddef.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/un.h>
#include <assert.h>
#include <pthread.h>

#include "libezcfg.h"
#include "libezcfg-private.h"

#define INVALID_SOCKET	-1
#define BUFFER_SIZE	4096
#define MAX_REQUEST_SIZE 8192

/*
 * ezcfg_worker:
 * Opaque object handling one event source.
 * Multi-threads model - worker part.
 */
struct ezcfg_worker {
	struct ezcfg *ezcfg;
	struct ezcfg_master *master;
	struct ezcfg_socket *client;
	unsigned char proto;
	void *proto_data;
	time_t birth_time;
	int64_t num_bytes_sent;
};

static void reset_per_request_attributes(struct ezcfg_worker *worker)
{
	if (worker->proto == EZCFG_PROTO_HTTP) {
	}
	else if (worker->proto == EZCFG_PROTO_IGRS) {
	}
	else if (worker->proto == EZCFG_PROTO_ISDP) {
	}
	else {
		/* unknown proto, do nothing */
	}
}

static void close_connection(struct ezcfg_worker *worker)
{
	struct ezcfg *ezcfg;

	assert(worker != NULL);

	ezcfg = worker->ezcfg;

	reset_per_request_attributes(worker);

	ezcfg_socket_close_sock(worker->client);
}

static void reset_connection_attributes(struct ezcfg_worker *worker) {
	reset_per_request_attributes(worker);
	worker->num_bytes_sent = 0;
	if (worker->proto == EZCFG_PROTO_HTTP) {
		ezcfg_http_reset_attributes(worker->proto_data);
	}
	if (worker->proto == EZCFG_PROTO_IGRS) {
		ezcfg_igrs_reset_attributes(worker->proto_data);
	}
}

/* Return content length of the request, or -1 constant if
 * Content-Length header is not set.
 */
static int get_content_length(const struct ezcfg_worker *worker) {
	const char *cl = NULL;
	if (worker->proto == EZCFG_PROTO_HTTP) {
		cl = ezcfg_http_get_header_value(worker->proto_data, EZCFG_HTTP_HEADER_CONTENT_LENGTH);
	}
	else if (worker->proto == EZCFG_PROTO_IGRS) {
		cl = ezcfg_igrs_get_http_header_value(worker->proto_data, EZCFG_IGRS_HEADER_CONTENT_LENGTH);
	}
	return cl == NULL ? -1 : strtol(cl, NULL, 10);
}


/* Check whether full request is buffered. Return:
 *   -1  if request is malformed
 *    0  if request is not yet fully buffered
 *   >0  actual request length, including last \r\n\r\n
 */
static int get_request_len(const char *buf, size_t buflen)
{
	const char *s, *e;
	int len = 0;

	for (s = buf, e = s + buflen - 1; len <= 0 && s < e; s++)
		/* Control characters are not allowed but >=128 is. */
		if (!isprint(* (unsigned char *) s) && *s != '\r' &&
		    *s != '\n' && * (unsigned char *) s < 128) {
			len = -1;
		} else if (s[0] == '\n' && s[1] == '\n') {
			len = (int) (s - buf) + 2;
		} else if (s[0] == '\n' && &s[1] < e &&
			   s[1] == '\r' && s[2] == '\n') {
			len = (int) (s - buf) + 3;
		}

	return len;
}

/**
 * Keep reading the input (either opened file descriptor fd, or socket sock,
 * or SSL descriptor ssl) into buffer buf, until \r\n\r\n appears in the
 * buffer (which marks the end of HTTP request). Buffer buf may already
 * have some data. The length of the data is stored in nread.
 * Upon every read operation, increase nread by the number of bytes read.
 **/
static int read_request(struct ezcfg_worker *worker, char *buf, int bufsiz, int *nread)
{
	struct ezcfg *ezcfg;
	int n, request_len;

	assert(worker != NULL);

	ezcfg = worker->ezcfg;

	request_len = 0;

	while (*nread < bufsiz && request_len == 0) {
		n = ezcfg_socket_read(worker->client, buf + *nread, bufsiz - *nread, 0);
		if (n <= 0) {
			break;
		} else {
			*nread += n;
			request_len = get_request_len(buf, (size_t) *nread);
		}
	}

	return request_len;
}

static bool error_handler(struct ezcfg_worker *worker)
{
	return false;
}

static int worker_printf(struct ezcfg_worker *worker, const char *fmt, ...)
{
	char buf[MAX_REQUEST_SIZE];
	int len;
	va_list ap;

	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	return ezcfg_socket_write(worker->client, buf, len, 0);
}

static void send_http_error(struct ezcfg_worker *worker, int status,
                            const char *reason, const char *fmt, ...)
{
	char buf[BUFFER_SIZE];
	va_list ap;
	int len;
	bool handled;

	ezcfg_http_set_status_code(worker->proto_data, status);
	handled = error_handler(worker);

	if (handled == false) {
		buf[0] = '\0';
		len = 0;

		/* Errors 1xx, 204 and 304 MUST NOT send a body */
		if (status > 199 && status != 204 && status != 304) {
			len = snprintf(buf, sizeof(buf),
			               "Error %d: %s\n", status, reason);
			va_start(ap, fmt);
			len += vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
			va_end(ap);
			worker->num_bytes_sent = len;
		}
		worker_printf(worker,
		              "HTTP/1.1 %d %s\r\n"
		              "Content-Type: text/plain\r\n"
		              "Content-Length: %d\r\n"
		              "Connection: close\r\n"
		              "\r\n%s", status, reason, len, buf);
	}
}

static void send_igrs_error(struct ezcfg_worker *worker, int status,
                            const char *reason, const char *fmt, ...)
{
	char buf[BUFFER_SIZE];
	va_list ap;
	int len;
	bool handled;

	//ezcfg_http_set_status_code(worker->proto_data, status);
	handled = error_handler(worker);

	if (handled == false) {
		buf[0] = '\0';
		len = 0;

		/* Errors 1xx, 204 and 304 MUST NOT send a body */
		if (status > 199 && status != 204 && status != 304) {
			len = snprintf(buf, sizeof(buf),
			               "Error %d: %s\n", status, reason);
			va_start(ap, fmt);
			len += vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
			va_end(ap);
			worker->num_bytes_sent = len;
		}
		worker_printf(worker,
		              "HTTP/1.1 %d %s\r\n"
		              "Content-Type: text/plain\r\n"
		              "Content-Length: %d\r\n"
		              "Connection: close\r\n"
		              "\r\n%s", status, reason, len, buf);
	}
}

/* This is the heart of the worker's logic.
 * This function is called when the request is read, parsed and validated,
 * and worker must decide what action to take: serve a file, or
 * a directory, or call embedded function, etcetera.
 */
static void handle_request(struct ezcfg_worker *worker)
{
	worker_printf(worker,
	             "HTTP/1.1 %d %s\r\n"
	             "\r\n", 200, "OK");
}

static void shift_to_next(struct ezcfg_worker *worker, char *buf, int req_len, int *nread)
{
	int cl;
	int over_len, body_len;

	cl = get_content_length(worker);
	over_len = *nread - req_len;
	assert(over_len >= 0);

	if (cl == -1) {
		body_len = 0;
	} else if (cl < (int64_t) over_len) {
		body_len = (int) cl;
	} else {
		body_len = over_len;
	}

	*nread -= req_len + body_len;
	memmove(buf, buf + req_len + body_len, *nread);
}

static void process_http_new_connection(struct ezcfg_worker *worker)
{
	int request_len, nread;
	char buf[MAX_REQUEST_SIZE];
	struct ezcfg *ezcfg;

	assert(worker != NULL);

	ezcfg = worker->ezcfg;

	nread = 0;
	reset_connection_attributes(worker);
	memset(buf, 0, sizeof(buf));

	/* If next request is not pipelined, read it */
	if ((request_len = get_request_len(buf, (size_t) nread)) == 0) {
		request_len = read_request(worker, buf, sizeof(buf), &nread);
	}
	assert(nread >= request_len);

	if (request_len <= 0) {
		err(ezcfg, "request error\n");
		return; /* Request is too large or format is not correct */
	}

	/* 0-terminate the request: parse http request uses sscanf
	 * !!! never, be careful not mangle the "\r\n\r\n" string!!!
	 */
	//buf[request_len - 1] = '\0';
	if (ezcfg_http_parse_request(worker->proto_data, buf) == true) {
		unsigned short major, minor;
		major = ezcfg_http_get_version_major(worker->proto_data);
		minor = ezcfg_http_get_version_minor(worker->proto_data);
		if (major != 1 || minor != 1) {
			send_http_error(worker, 505,
			                "HTTP version not supported",
			                "%s", "Weird HTTP version");
		} else {
			ezcfg_http_set_message_body(worker->proto_data, buf + request_len, nread - request_len);
			worker->birth_time = time(NULL);
			ezcfg_http_dump(worker->proto_data);
			handle_request(worker);
			shift_to_next(worker, buf, request_len, &nread);
		}
	} else {
		/* Do not put garbage in the access log */
		send_http_error(worker, 400, "Bad Request", "Can not parse request: %.*s", nread, buf);
	}
}

static void process_igrs_new_connection(struct ezcfg_worker *worker)
{
	int request_len, nread;
	char buf[MAX_REQUEST_SIZE];
	struct ezcfg *ezcfg;

	assert(worker != NULL);

	ezcfg = worker->ezcfg;

	nread = 0;
	reset_connection_attributes(worker);
	memset(buf, 0, sizeof(buf));

	/* If next request is not pipelined, read it */
	if ((request_len = get_request_len(buf, (size_t) nread)) == 0) {
		request_len = read_request(worker, buf, sizeof(buf), &nread);
	}
	assert(nread >= request_len);

	if (request_len <= 0) {
		err(ezcfg, "request error\n");
		return; /* Request is too large or format is not correct */
	}

	/* 0-terminate the request: parse http request uses sscanf
	 * !!! never, be careful not mangle the "\r\n\r\n" string!!!
	 */
	//buf[request_len - 1] = '\0';
	if (ezcfg_igrs_parse_request(worker->proto_data, buf) == true) {
		unsigned short major, minor;
		major = ezcfg_igrs_get_version_major(worker->proto_data);
		minor = ezcfg_igrs_get_version_minor(worker->proto_data);
		if (major != 1 || minor != 0) {
			send_igrs_error(worker, 505,
			                "IGRS version not supported",
			                "%s", "Weird IGRS version");
		} else {
			ezcfg_igrs_set_message_body(worker->proto_data, buf + request_len, nread - request_len);
			worker->birth_time = time(NULL);
			ezcfg_igrs_dump(worker->proto_data);
			handle_request(worker);
			shift_to_next(worker, buf, request_len, &nread);
		}
	} else {
		/* Do not put garbage in the access log */
		send_igrs_error(worker, 400, "Bad Request", "Can not parse request: %.*s", nread, buf);
	}
}

static void process_new_connection(struct ezcfg_worker *worker)
{
	struct ezcfg *ezcfg;

	assert(worker != NULL);

	ezcfg = worker->ezcfg;

	/* dispatch protocol handler */
	if (worker->proto == EZCFG_PROTO_HTTP) {
		process_http_new_connection(worker);
	}
	else if (worker->proto == EZCFG_PROTO_IGRS) {
		process_igrs_new_connection(worker);
	}
}

struct ezcfg_worker *ezcfg_worker_new(struct ezcfg_master *master)
{
	struct ezcfg *ezcfg;
	struct ezcfg_worker *worker = NULL;
	struct ezcfg_socket *client = NULL;

	assert(master != NULL);
	worker = calloc(1, sizeof(struct ezcfg_worker));
	if (worker == NULL)
		return NULL;

	ezcfg = ezcfg_master_get_ezcfg(master);
	client = ezcfg_socket_calloc(ezcfg, 1);
	if (client == NULL) {
		free(worker);
		return NULL;
	}

	memset(worker, 0, sizeof(struct ezcfg_worker));

	worker->ezcfg = ezcfg;
	worker->master = master;
	worker->client = client;
	worker->proto = EZCFG_PROTO_UNKNOWN;
	worker->proto_data = NULL;
	return worker;

}

void ezcfg_worker_thread(struct ezcfg_worker *worker) 
{
	struct ezcfg *ezcfg;

	assert(worker != NULL);

	ezcfg = worker->ezcfg;

	while ((ezcfg_master_is_stop(worker->master) == false) &&
	       (ezcfg_master_get_socket(worker->master, worker->client) == true)) {

		worker->birth_time = time(NULL);

		/* set communication protocol */
		worker->proto = ezcfg_socket_get_proto(worker->client);

		/* initialize protocol data structure */
		if (worker->proto == EZCFG_PROTO_HTTP) {
			worker->proto_data = ezcfg_http_new(ezcfg);
		}
		else if (worker->proto == EZCFG_PROTO_IGRS) {
			worker->proto_data = ezcfg_igrs_new(ezcfg);
		}
		else if (worker->proto == EZCFG_PROTO_ISDP) {
			//worker->proto_data = ezcfg_isdp_new(ezcfg);
		}

		/* process the connection */
		if (worker->proto_data != NULL) {
			process_new_connection(worker);
		}

		/* close connection */
		close_connection(worker);

		/* release protocol data structure */
		if (worker->proto == EZCFG_PROTO_HTTP) {
			ezcfg_http_delete(worker->proto_data);
		}
		else if (worker->proto == EZCFG_PROTO_IGRS) {
			ezcfg_igrs_delete(worker->proto_data);
		}
		else if (worker->proto == EZCFG_PROTO_ISDP) {
			//ezcfg_isdp_delete(worker->proto_data);
		}
	}

	/* Signal master that we're done with connection and exiting */
	ezcfg_master_stop_worker(worker->master);
}
