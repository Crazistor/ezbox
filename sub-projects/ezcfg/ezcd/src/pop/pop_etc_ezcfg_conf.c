/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : pop_etc_ezcfg_conf.c
 *
 * Description  : ezbox /etc/ezcfg.conf generating program
 *
 * Copyright (C) 2010 by ezbox-project
 *
 * History      Rev       Description
 * 2010-11-02   0.1       Write it from scratch
 * ============================================================================
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <stdarg.h>

#include "ezcd.h"

int pop_etc_ezcfg_conf(int flag)
{
	FILE *file;
	char buf[256];
	int rc;

	switch (flag) {
	case RC_BOOT :
		/* use default config for boot stage */
		break;

	case RC_START :
	case RC_RELOAD :
	case RC_RESTART :
		/* get ezcd config from nvram */
		file = fopen("/etc/ezcfg.conf", "w");
		if (file == NULL)
			return (EXIT_FAILURE);

		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EZCFG, NVRAM_BUFFER_SIZE), buf, sizeof(buf));
		if (rc == 0) {
			fprintf(file, "%s=%s\n", SERVICE_OPTION(EZCFG, NVRAM_BUFFER_SIZE), buf);
		}

		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EZCFG, NVRAM_BACKEND_TYPE), buf, sizeof(buf));
		if (rc == 0) {
			fprintf(file, "%s=%s\n", SERVICE_OPTION(EZCFG, NVRAM_BACKEND_TYPE), buf);
		}

		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EZCFG, NVRAM_STORAGE_PATH), buf, sizeof(buf));
		if (rc == 0) {
			fprintf(file, "%s=%s\n", SERVICE_OPTION(EZCFG, NVRAM_STORAGE_PATH), buf);
		}

		fclose(file);
		break;
	}

	return (EXIT_SUCCESS);
}
