/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : init.c
 *
 * Description  : ezbox initramfs preinit program
 *
 * Copyright (C) 2010 by ezbox-project
 *
 * History      Rev       Description
 * 2010-06-13   0.1       Write it from scratch
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

int preinit_main(int argc, char **argv)
{
        FILE *file = NULL;
	char *init_argv[] = { "/sbin/init", NULL };
	int ret = 0;
	char kver[32] = "";

	/* unset umask */
	umask(0);

	/* /proc */
	mkdir("/proc", 0555);
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);

	/* sysfs -> /sys */
	mkdir("/sys", 0755);
	mount("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);

	/* get kernel version */
	file = fopen("/proc/version", "r");
	if (file != NULL)
	{
		if (fgets(kver, 15, file) != NULL)
		{
			if (!strcmp(kver, "Linux version "))
			{
				if (fgets(kver, sizeof(kver), file) != NULL)
				{
					char *p = strchr(kver, ' ');
					if (p)
					{
						*p = '\0';
					}
				}
				else
				{
					kver[0] = '\0';
				}
			}
			else
			{
				kver[0] = '\0';
			}
		}
		fclose(file);
	}


	/* /dev */
	mkdir("/dev", 0755);
	// mount("tmpfs", "/dev", "tmpfs", MS_MGC_VAL, NULL);

	/* /etc */
	mkdir("/etc", 0755);

	/* /tmp */
	mkdir("/tmp", 0777);

	/* /var */
	mkdir("/var", 0777);

	/* hotplug2 */
	mknod("/dev/console", S_IRWXU|S_IFCHR, makedev(5, 1));
	ret = system("/sbin/hotplug2 --set-worker /lib/hotplug2/worker_fork.so --set-rules-file /etc/hotplug2-init.rules --no-persistent --set-coldplug-cmd /sbin/udevtrigger");
	ret = system("/sbin/hotplug2 --set-worker /lib/hotplug2/worker_fork.so --set-rules-file /etc/hotplug2-init.rules --persistent &");

	/* init shms */
	mkdir("/dev/shm", 0777);

	/* Mount /dev/pts */
	mkdir("/dev/pts", 0777);
	mount("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);

	/* init hotplug */
	file = fopen("/proc/sys/kernel/hotplug", "w");
	if (file != NULL)
	{
		fprintf(file, "%s", "");
		fclose(file);
	}

	/* load preinit kernel modules */
	file = fopen("/etc/modules", "r");
	if (file != NULL)
	{
		char cmd[64];
		char buf[32];
		while(fgets(buf, sizeof(buf), file) != NULL)
		{
			if(buf[0] != '#')
			{
				int len = strlen(buf);
				while((len > 0) && 
				      (buf[len] == '\0' || 
				       buf[len] == '\r' || 
				       buf[len] == '\n'))
				{
					buf[len] = '\0';
					len --;
				}
				if(len > 0)
				{
					snprintf(cmd, sizeof(cmd), "insmod /lib/modules/%s/%s.ko", kver, buf);
					ret = system(cmd);
				}
			}
		}
		fclose(file);
	}

	/* switch to /sbin/init */
	execv(init_argv[0], init_argv);

	return (EXIT_SUCCESS);
}
