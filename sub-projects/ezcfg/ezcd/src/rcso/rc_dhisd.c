/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : rc_dhisd.c
 *
 * Description  : ezbox run DHIS server service
 *
 * Copyright (C) 2008-2013 by ezbox-project
 *
 * History      Rev       Description
 * 2012-10-03   0.1       Write it from scratch
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

#ifdef _EXEC_
int main(int argc, char **argv)
#else
int rc_dhisd(int argc, char **argv)
#endif
{
	char buf[128];
	struct stat stat_buf;
	int rc;
	int flag, ret;

	if (argc < 2) {
		return (EXIT_FAILURE);
	}

	if (strcmp(argv[0], "dhisd")) {
		return (EXIT_FAILURE);
	}

	if (utils_init_ezcfg_api(EZCD_CONFIG_FILE_PATH) == false) {
		return (EXIT_FAILURE);
	}

	flag = utils_get_rc_act_type(argv[1]);

	switch (flag) {
	case RC_ACT_RESTART :
	case RC_ACT_STOP :
		utils_system("start-stop-daemon -K -n dhisd");
		if (flag == RC_ACT_STOP) {
			ret = EXIT_SUCCESS;
			break;
		}

		/* RC_ACT_RESTART fall through */
	case RC_ACT_START :
		rc = utils_nvram_cmp(NVRAM_SERVICE_OPTION(RC, DHISD_ENABLE), "1");
		if (rc < 0) {
			return (EXIT_FAILURE);
		}

		pop_etc_dhisd_conf(RC_ACT_START);
		pop_etc_dhis_db(RC_ACT_START);

		/* mkdir for /var/run/dhis/ */
		if ((stat("/var/run/dhis", &stat_buf) != 0) ||
		    (S_ISDIR(stat_buf.st_mode) == 0)) {
			snprintf(buf, sizeof(buf), "%s -rf /var/run/dhis", CMD_RM);
			utils_system(buf);
			mkdir("/var/run/dhis", 0755);
		}

		/* mkdir for /var/log/dhis/ */
		if ((stat("/var/log/dhis", &stat_buf) != 0) ||
		    (S_ISDIR(stat_buf.st_mode) == 0)) {
			snprintf(buf, sizeof(buf), "%s -rf /var/log/dhis", CMD_RM);
			utils_system(buf);
			mkdir("/var/log/dhis", 0755);
		}

		utils_system("start-stop-daemon -S -n dhisd -a " CMD_DHID);
		ret = EXIT_SUCCESS;
		break;

	default :
		ret = EXIT_FAILURE;
		break;
	}
	return (ret);
}
