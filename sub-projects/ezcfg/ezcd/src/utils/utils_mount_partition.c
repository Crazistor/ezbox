/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : utils_mount_partition.c
 *
 * Description  : ezbox mount partition
 *
 * Copyright (C) 2008-2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-07-28   0.1       Write it from scratch
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
#include "rc_func.h"
#include "utils.h"

#if 0
#define DBG(format, args...) do {\
	FILE *fp = fopen("/dev/kmsg", "a"); \
	if (fp) { \
		fprintf(fp, format, ## args); \
		fclose(fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

#define DBG2() do {\
	pid_t pid = getpid(); \
	FILE *fp = fopen("/dev/kmsg", "a"); \
	if (fp) { \
		char buf[32]; \
		FILE *fp2; \
		int i; \
		for(i=pid; i<pid+30; i++) { \
			snprintf(buf, sizeof(buf), "/proc/%d/stat", i); \
			fp2 = fopen(buf, "r"); \
			if (fp2) { \
				if (fgets(buf, sizeof(buf)-1, fp2) != NULL) { \
					fprintf(fp, "pid=[%d] buf=%s\n", i, buf); \
				} \
				fclose(fp2); \
			} \
		} \
		fclose(fp); \
	} \
} while(0)

static int install_fs_modules(char *fs_type)
{
	int rc;
	if (strcmp(fs_type, "ntfs-3g") == 0) {
		rc = utils_install_kernel_module("fuse", NULL);
	}
	else if (strcmp(fs_type, "vfat") == 0) {
		rc = utils_install_kernel_module("vfat", NULL);
	}
	else if (strcmp(fs_type, "fat") == 0) {
		rc = utils_install_kernel_module("fat", NULL);
	}
	return rc;
}

int utils_mount_partition(char *dev, char *path, char *fs_type, char *args)
{
	char buf[128];
	char *p;

	if (dev == NULL || path == NULL)
		return (EXIT_FAILURE);

	p = (args == NULL) ? "" : args;

	if (fs_type != NULL) {
		install_fs_modules(fs_type);
		if (strcmp(fs_type, "ntfs-3g") == 0) {
			snprintf(buf, sizeof(buf), "%s %s %s %s", "/usr/bin/ntfs-3g", p, dev, path);
		}
		else {
			snprintf(buf, sizeof(buf), "%s %s -t %s %s %s", CMD_MOUNT, p, fs_type, dev, path);
		}
	}
	else {
		snprintf(buf, sizeof(buf), "%s %s %s %s", CMD_MOUNT, p, dev, path);
	}
	system(buf);
	return (EXIT_SUCCESS);
}
