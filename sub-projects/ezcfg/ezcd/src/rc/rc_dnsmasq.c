/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : rc_dnsmasq.c
 *
 * Description  : ezbox run dns & dhcp service
 *
 * Copyright (C) 2008-2011 by ezbox-project
 *
 * History      Rev       Description
 * 2010-11-17   0.1       Write it from scratch
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
#include "pop_func.h"

int rc_dnsmasq(int flag)
{
	int rc;

	if ((utils_service_binding_lan(NVRAM_SERVICE_OPTION(RC, DNSMASQ_BINDING)) == false) &&
	   (utils_service_binding_wan(NVRAM_SERVICE_OPTION(RC, DNSMASQ_BINDING)) == false)) {
		return (EXIT_FAILURE);
	}

	switch (flag) {
	case RC_START :
		rc = nvram_match(NVRAM_SERVICE_OPTION(RC, DNSMASQ_ENABLE), "1");
		if (rc < 0) {
			return (EXIT_FAILURE);
		}

		pop_etc_dnsmasq_conf(RC_START);
		system("start-stop-daemon -S -n dnsmasq -a /usr/sbin/dnsmasq");
		break;

	case RC_STOP :
		system("start-stop-daemon -K -n dnsmasq");
		break;

	case RC_RESTART :
		rc = rc_dnsmasq(RC_STOP);
		rc = rc_dnsmasq(RC_START);
		break;
	}
	return (EXIT_SUCCESS);
}
