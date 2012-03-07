/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : rc_lan.c
 *
 * Description  : ezbox run network LAN config service
 *
 * Copyright (C) 2008-2012 by ezbox-project
 *
 * History      Rev       Description
 * 2010-11-04   0.1       Write it from scratch
 * 2011-10-06   0.2       Modify it to use rcso framework
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
#include <net/if.h>

#include "ezcd.h"

#ifdef _EXEC_
int main(int argc, char **argv)
#else
int rc_lan(int argc, char **argv)
#endif
{
	char lan_ifname[IFNAMSIZ];
	char cmdline[256];
	int flag, ret;

	if (argc < 2) {
		return (EXIT_FAILURE);
	}

	if (strcmp(argv[0], "lan")) {
		return (EXIT_FAILURE);
	}

	ret = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(LAN, IFNAME), lan_ifname, sizeof(lan_ifname));
	if (ret < 0)
		return (EXIT_FAILURE);

	if (utils_init_ezcfg_api(EZCD_CONFIG_FILE_PATH) == false) {
		return (EXIT_FAILURE);
	}

	flag = utils_get_rc_act_type(argv[1]);

	switch (flag) {
	case RC_ACT_RESTART :
	case RC_ACT_STOP :
		/* bring down LAN interface */
		snprintf(cmdline, sizeof(cmdline), "%s %s down", CMD_IFDOWN, lan_ifname);
		ret = utils_system(cmdline);
		if (flag == RC_ACT_STOP) {
			ret = EXIT_SUCCESS;
			break;
		}

		/* RC_ACT_RESTART fall through */
	case RC_ACT_BOOT :
	case RC_ACT_START :
		/* bring up LAN interface and config it */
		snprintf(cmdline, sizeof(cmdline), "%s %s up", CMD_IFUP, lan_ifname);
		ret = utils_system(cmdline);
		ret = EXIT_SUCCESS;
		break;

	default :
		ret = EXIT_FAILURE;
		break;
	}

	return (ret);
}
