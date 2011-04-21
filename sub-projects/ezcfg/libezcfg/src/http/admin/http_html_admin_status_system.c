/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : http/admin/http_html_admin_status_system.c
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-04-11   0.1       Write it from scratch
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <pthread.h>

#include <locale.h>
#include <libintl.h>

#include "ezcfg.h"
#include "ezcfg-private.h"
#include "ezcfg-http.h"
#include "ezcfg-html.h"

/**
 * Private functions
 **/
static int set_html_main_status_system(
	struct ezcfg_http *http,
	struct ezcfg_nvram *nvram,
	struct ezcfg_locale *locale,
	struct ezcfg_html *html,
	int pi, int si)
{
	struct ezcfg *ezcfg;
	int main_index;
	int content_index, child_index;
	char buf[1024];
	char *p = NULL;
	struct sysinfo info;
	time_t tm;
	struct tm tms;
	bool tz_ok = false;
	bool ct_ok = false;
	int ret = -1;

	ASSERT(http != NULL);
	ASSERT(nvram != NULL);
	ASSERT(html != NULL);
	ASSERT(pi > 1);

	ezcfg = html->ezcfg;

	/* <div id="main"> */
	main_index = ezcfg_html_add_body_child(html, pi, si, EZCFG_HTML_DIV_ELEMENT_NAME, NULL);
	if (main_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, main_index, EZCFG_HTML_ID_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_DIV_ID_MAIN, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* <div id="menu"> */
	child_index = ezcfg_http_html_admin_set_html_menu(http, nvram, html, main_index, -1);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_http_html_admin_set_html_menu.\n");
		goto func_exit;
	}

	/* <div id="content"> */
	content_index = ezcfg_html_add_body_child(html, main_index, child_index, EZCFG_HTML_DIV_ELEMENT_NAME, NULL);
	if (content_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, content_index, EZCFG_HTML_ID_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_DIV_ID_CONTENT, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	child_index = -1;
	/* <h3>Device Information</h3> */
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_H3_ELEMENT_NAME, ezcfg_locale_text(locale, "Device Information"));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* <p>Device Name : ezbox </p> */
	ezcfg_nvram_get_entry_value(nvram, NVRAM_SERVICE_OPTION(SYS, DEVICE_NAME), &p);
	snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%s",
		ezcfg_locale_text(locale, "Device Name"),
		(p != NULL) ? p : ezcfg_locale_text(locale, "Unknown Device"));
	if (p != NULL) {
		free(p);
	}
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* <p>Serial Number : 0123456789 </p> */
	ezcfg_nvram_get_entry_value(nvram, NVRAM_SERVICE_OPTION(SYS, SERIAL_NUMBER), &p);
	snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%s",
		ezcfg_locale_text(locale, "Serial Number"),
		(p != NULL) ? p : ezcfg_locale_text(locale, "Invalid Serial Number"));
	if (p != NULL) {
		free(p);
	}
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* <p>Hardware Version : 1.0 </p> */
	ezcfg_nvram_get_entry_value(nvram, NVRAM_SERVICE_OPTION(SYS, HARDWARE_VERSION), &p);
	snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%s",
		ezcfg_locale_text(locale, "Hardware Version"),
		(p != NULL) ? p : ezcfg_locale_text(locale, "Invalid Version"));
	if (p != NULL) {
		free(p);
	}
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* <p>Software Version : 1.0 </p> */
	ezcfg_nvram_get_entry_value(nvram, NVRAM_SERVICE_OPTION(SYS, SOFTWARE_VERSION), &p);
	snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%s",
		ezcfg_locale_text(locale, "Software Version"),
		(p != NULL) ? p : ezcfg_locale_text(locale, "Invalid Version"));
	if (p != NULL) {
		free(p);
	}
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* <h3>System Configurations</h3> */
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_H3_ELEMENT_NAME, ezcfg_locale_text(locale, "System Configurations"));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* <p>Time Zone : Asia/Shanghai</p> */
	ezcfg_nvram_get_entry_value(nvram, NVRAM_SERVICE_OPTION(SYS, TIME_ZONE), &p);
	snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%s",
		ezcfg_locale_text(locale, "Time Zone"),
		(p != NULL) ? ezcfg_locale_text(locale, p) : ezcfg_locale_text(locale, "Unknown Time Zone"));
	if (p != NULL) {
		free(p);
		tz_ok = true;
	}
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}
	if (tz_ok == false) {
		ezcfg_html_add_body_child_attribute(html, child_index, EZCFG_HTML_CLASS_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_P_CLASS_WARNING, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);
	}

	/* <p>Current Time : Thu Apr 21 18:10:50 2011</p> */
	time(&tm);
	memcpy(&tms, localtime(&tm), sizeof(struct tm));
	p = NULL;
	if ((tz_ok == true) &&
	    (time(0) > (unsigned long)60*60*24*365)) {
		p = malloc(256);
		if (p != NULL) {
			strftime(p, 255, "%c", &tms);
		}
	}
	snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%s",
		ezcfg_locale_text(locale, "Current Time"),
		(p != NULL) ? p : ezcfg_locale_text(locale, "Not Available"));
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
	if (p != NULL) {
		free(p);
		ct_ok = true;
	}
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}
	if (ct_ok == false) {
		ezcfg_html_add_body_child_attribute(html, child_index, EZCFG_HTML_CLASS_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_P_CLASS_WARNING, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);
	}

	/* <h3>Running Status</h3> */
	child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_H3_ELEMENT_NAME, ezcfg_locale_text(locale, "Running Status"));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	if (sysinfo(&info) == -1) {
		/* access system info error */
		/* <p>System running status is not available right now...</p> */
		child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, ezcfg_locale_text(locale, "System running status is not available right now, please check it later."));
		if (child_index < 0) {
			err(ezcfg, "ezcfg_html_add_body_child error.\n");
			goto func_exit;
		}
		ezcfg_html_add_body_child_attribute(html, child_index, EZCFG_HTML_CLASS_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_P_CLASS_WARNING, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);
	}
	else {
		if (info.mem_unit == 1) {
			info.mem_unit = 1024;
			/* TODO: Make all this stuff not overflow when mem >= 4 GiB */
			info.totalram /= info.mem_unit;
			info.freeram /= info.mem_unit;
			info.sharedram /= info.mem_unit;
			info.bufferram /= info.mem_unit;
			info.totalswap /= info.mem_unit;
			info.freeswap /= info.mem_unit;
		}
		else {
			info.mem_unit /= 1024;
			/* TODO: Make all this stuff not overflow when mem >= 4 GiB */
			info.totalram *= info.mem_unit;
			info.freeram *= info.mem_unit;
			info.sharedram *= info.mem_unit;
			info.bufferram *= info.mem_unit;
			info.totalswap *= info.mem_unit;
			info.freeswap *= info.mem_unit;
		}

		/* <p>Up Time : 10000 seconds </p> */
		snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%ld&nbsp;%s",
			ezcfg_locale_text(locale, "Up Time"),
			info.uptime,
			ezcfg_locale_text(locale, "seconds"));
		child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
		if (child_index < 0) {
			err(ezcfg, "ezcfg_html_add_body_child error.\n");
			goto func_exit;
		}

		/* <p>Total Memory : 65536 KB </p> */
		snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%lu&nbsp;%s",
			ezcfg_locale_text(locale, "Total Memory"),
			info.totalram,
			ezcfg_locale_text(locale, "KB"));
		child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
		if (child_index < 0) {
			err(ezcfg, "ezcfg_html_add_body_child error.\n");
			goto func_exit;
		}

		/* <p>Free Memory : 16384 KB </p> */
		snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%lu&nbsp;%s",
			ezcfg_locale_text(locale, "Free Memory"),
			info.freeram,
			ezcfg_locale_text(locale, "KB"));
		child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
		if (child_index < 0) {
			err(ezcfg, "ezcfg_html_add_body_child error.\n");
			goto func_exit;
		}

		/* <p>Total Swap : 65536 KB </p> */
		snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%lu&nbsp;%s",
			ezcfg_locale_text(locale, "Total swap"),
			info.totalswap,
			ezcfg_locale_text(locale, "KB"));
		child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
		if (child_index < 0) {
			err(ezcfg, "ezcfg_html_add_body_child error.\n");
			goto func_exit;
		}

		/* <p>Free Swap : 16384 KB </p> */
		snprintf(buf, sizeof(buf), "%s&nbsp;:&nbsp;%lu&nbsp;%s",
			ezcfg_locale_text(locale, "Free Swap"),
			info.freeswap,
			ezcfg_locale_text(locale, "KB"));
		child_index = ezcfg_html_add_body_child(html, content_index, child_index, EZCFG_HTML_P_ELEMENT_NAME, buf);
		if (child_index < 0) {
			err(ezcfg, "ezcfg_html_add_body_child error.\n");
			goto func_exit;
		}
	}

	/* <input> buttons part */
	child_index = ezcfg_http_html_admin_set_html_button(http, nvram, html, content_index, child_index);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_http_html_admin_set_html_button.\n");
		goto func_exit;
	}

	/* must return main index */
	ret = main_index;

func_exit:
	return ret;
}

static int build_admin_status_system_response(struct ezcfg_http *http, struct ezcfg_nvram *nvram)
{
	struct ezcfg *ezcfg;
	struct ezcfg_html *html = NULL;
	struct ezcfg_locale *locale = NULL;
	int head_index, body_index, child_index;
	int container_index, form_index;
	char *msg = NULL;
	int msg_len;
	int n;
	int rc = 0;
	bool ret;
	
	ASSERT(http != NULL);
	ASSERT(nvram != NULL);

	ezcfg = http->ezcfg;

	/* set locale info */
	locale = ezcfg_locale_new(ezcfg);
	if (locale != NULL) {
		ezcfg_locale_set_domain(locale, EZCFG_HTTP_HTML_ADMIN_STATUS_SYSTEM_DOMAIN);
		ezcfg_locale_set_dir(locale, EZCFG_HTTP_HTML_LANG_DIR);
	}

	html = ezcfg_html_new(ezcfg);

	if (html == NULL) {
		err(ezcfg, "can not alloc html.\n");
		rc = -1;
		goto func_exit;
	}

	/* clean HTML structure info */
	ezcfg_html_reset_attributes(html);

	/* build HTML */
	ezcfg_html_set_version_major(html, 4);
	ezcfg_html_set_version_minor(html, 1);

	/* HTML root */
	rc = ezcfg_html_set_root(html, EZCFG_HTML_HTML_ELEMENT_NAME);
	if (rc < 0) {
		err(ezcfg, "ezcfg_html_set_root.\n");
		rc = -1;
		goto func_exit;
	}

	/* HTML Head */
	head_index = ezcfg_html_set_head(html, EZCFG_HTML_HEAD_ELEMENT_NAME);
	if (head_index < 0) {
		err(ezcfg, "ezcfg_html_set_head error.\n");
		rc = -1;
		goto func_exit;
	}

	/* set admin common Head */
	child_index = ezcfg_http_html_admin_set_html_common_head(html, head_index, -1);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_http_html_admin_set_html_common_head error.\n");
		rc = -1;
		goto func_exit;
	}

	/* HTML Title */
	child_index = ezcfg_html_add_head_child(html, head_index, child_index, EZCFG_HTML_TITLE_ELEMENT_NAME, ezcfg_locale_text(locale, "System Status"));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_head_child error.\n");
		rc = -1;
		goto func_exit;
	}

	/* HTML Body */
	body_index = ezcfg_html_set_body(html, EZCFG_HTML_BODY_ELEMENT_NAME);
	if (body_index < 0) {
		err(ezcfg, "ezcfg_html_set_body error.\n");
		rc = -1;
		goto func_exit;
	}

	/* HTML div container */
	container_index = ezcfg_html_add_body_child(html, body_index, -1, EZCFG_HTML_DIV_ELEMENT_NAME, NULL);
	if (container_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		rc = -1;
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, container_index, EZCFG_HTML_ID_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_DIV_ID_CONTAINER, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* HTML form */
	form_index = ezcfg_html_add_body_child(html, container_index, -1, EZCFG_HTML_FORM_ELEMENT_NAME, NULL);
	if (form_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		rc = -1;
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, form_index, EZCFG_HTML_NAME_ATTRIBUTE_NAME, "status_system", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);
	ezcfg_html_add_body_child_attribute(html, form_index, EZCFG_HTML_METHOD_ATTRIBUTE_NAME, "post", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);
	ezcfg_html_add_body_child_attribute(html, form_index, EZCFG_HTML_ACTION_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_PREFIX_URI "status_system", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* HTML div head */
	child_index = ezcfg_http_html_admin_set_html_head(http, nvram, html, form_index, -1);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_http_html_admin_set_html_head error.\n");
		rc = -1;
		goto func_exit;
	}

	/* HTML div main */
	child_index = set_html_main_status_system(http, nvram, locale, html, form_index, child_index);
	if (child_index < 0) {
		err(ezcfg, "set_html_main_status_system error.\n");
		rc = -1;
		goto func_exit;
	}

	/* HTML div foot */
	child_index = ezcfg_http_html_admin_set_html_foot(http, nvram, html, form_index, child_index);
	if (child_index < 0) {
		err(ezcfg, "ezcfg_http_html_admin_set_html_foot error.\n");
		rc = -1;
		goto func_exit;
	}

	msg_len = ezcfg_html_get_message_length(html);
	if (msg_len < 0) {
		err(ezcfg, "ezcfg_html_get_message_length\n");
		rc = -1;
		goto func_exit;
	}
	msg_len++; /* one more for '\0' */
	msg = (char *)malloc(msg_len);
	if (msg == NULL) {
		err(ezcfg, "malloc error.\n");
		rc = -1;
		goto func_exit;
	}

	memset(msg, 0, msg_len);
	n = ezcfg_html_write_message(html, msg, msg_len);
	if (n < 0) {
		err(ezcfg, "ezcfg_html_write_message\n");
		rc = -1;
		goto func_exit;
	}

	/* FIXME: name point to http->request_uri !!!
         * never reset http before using name */
	/* clean http structure info */
	ezcfg_http_reset_attributes(http);
	ezcfg_http_set_status_code(http, 200);
	ezcfg_http_set_state_response(http);

	if (ezcfg_http_set_message_body(http, msg, n) == NULL) {
		err(ezcfg, "ezcfg_http_set_message_body\n");
		rc = -1;
		goto func_exit;
	}

	ret = ezcfg_http_html_admin_set_http_html_common_header(http);
	if (ret == false) {
		err(ezcfg, "ezcfg_http_html_admin_set_http_html_common_header error.\n");
		rc = -1;
		goto func_exit;
	}

	/* set return value */
	rc = 0;
func_exit:
	if (locale != NULL)
		ezcfg_locale_delete(locale);

	if (html != NULL)
		ezcfg_html_delete(html);

	if (msg != NULL)
		free(msg);

	return rc;
}

/**
 * Public functions
 **/

int ezcfg_http_html_admin_status_system_handler(struct ezcfg_http *http, struct ezcfg_nvram *nvram)
{
	struct ezcfg *ezcfg;
	int ret = -1;

	ASSERT(http != NULL);
	ASSERT(nvram != NULL);

	ezcfg = http->ezcfg;

	/* admin status_system uri=[/admin/status_system] */
	ret = build_admin_status_system_response(http, nvram);
	return ret;
}
