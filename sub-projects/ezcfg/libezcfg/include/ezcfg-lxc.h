/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : ezcfg-lxc.h
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2008-2013 by ezbox-project
 *
 * History      Rev       Description
 * 2012-07-23   0.1       Write it from scratch
 * ============================================================================
 */

#ifndef _EZCFG_LXC_H_
#define _EZCFG_LXC_H_

/* ezcfg nvram name prefix */
#define EZCFG_LXC_NVRAM_PREFIX              "lxc."

#define EZCFG_LXC_CONTAINER_NUMBER          "container_number"

/* keywords */
#define EZCFG_LXC_CONF_KEYWORD_LXC_ARCH     "lxc.arch"
#define EZCFG_LXC_CONF_KEYWORD_LXC_UTSNAME  "lxc.utsname"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_TYPE \
	"lxc.network.type"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_FLAGS \
	"lxc.network.flags"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_LINK \
	"lxc.network.link"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_NAME \
	"lxc.network.name"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_HWADDR \
	"lxc.network.hwaddr"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_IPV4 \
	"lxc.network.ipv4"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_IPV6 \
	"lxc.network.ipv6"
#define EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_SCRIPT_UP \
	"lxc.network.script.up"
#define EZCFG_LXC_CONF_KEYWORD_LXC_PTS      "lxc.pts"
#define EZCFG_LXC_CONF_KEYWORD_LXC_CONSOLE  "lxc.console"
#define EZCFG_LXC_CONF_KEYWORD_LXC_TTY      "lxc.tty"
#define EZCFG_LXC_CONF_KEYWORD_LXC_DEVTTYDIR \
	"lxc.devttydir"
#define EZCFG_LXC_CONF_KEYWORD_LXC_MOUNT    "lxc.mount"
#define EZCFG_LXC_CONF_KEYWORD_LXC_MOUNT_ENTRY \
	"lxc.mount.entry"
#define EZCFG_LXC_CONF_KEYWORD_LXC_ROOTFS   "lxc.rootfs"
#define EZCFG_LXC_CONF_KEYWORD_LXC_ROOTFS_MOUNT \
	"lxc.rootfs.mount"
#define EZCFG_LXC_CONF_KEYWORD_LXC_PIVOTDIR "lxc.pivotdir"
#define EZCFG_LXC_CONF_KEYWORD_LXC_CGROUP_CPUSET_CPUS \
	"lxc.cgroup.cpuset.cpus"
#define EZCFG_LXC_CONF_KEYWORD_LXC_CAP_DROP "lxc.cap.drop"

#define EZCFG_LXC_CONF_1_LXC_ARCH           "c1." EZCFG_LXC_CONF_KEYWORD_LXC_ARCH
#define EZCFG_LXC_CONF_1_LXC_UTSNAME        "c1." EZCFG_LXC_CONF_KEYWORD_LXC_UTSNAME
#define EZCFG_LXC_CONF_1_LXC_NETWORK_TYPE   "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_TYPE
#define EZCFG_LXC_CONF_1_LXC_NETWORK_FLAGS  "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_FLAGS
#define EZCFG_LXC_CONF_1_LXC_NETWORK_LINK   "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_LINK
#define EZCFG_LXC_CONF_1_LXC_NETWORK_NAME   "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_NAME
#define EZCFG_LXC_CONF_1_LXC_NETWORK_HWADDR "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_HWADDR
#define EZCFG_LXC_CONF_1_LXC_NETWORK_IPV4   "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_IPV4
#define EZCFG_LXC_CONF_1_LXC_NETWORK_IPV6   "c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_IPV6
#define EZCFG_LXC_CONF_1_LXC_NETWORK_SCRIPT_UP \
	"c1." EZCFG_LXC_CONF_KEYWORD_LXC_NETWORK_SCRIPT_UP
#define EZCFG_LXC_CONF_1_LXC_PTS            "c1." EZCFG_LXC_CONF_KEYWORD_LXC_PTS
#define EZCFG_LXC_CONF_1_LXC_CONSOLE        "c1." EZCFG_LXC_CONF_KEYWORD_LXC_CONSOLE
#define EZCFG_LXC_CONF_1_LXC_TTY            "c1." EZCFG_LXC_CONF_KEYWORD_LXC_TTY
#define EZCFG_LXC_CONF_1_LXC_DEVTTYDIR      "c1." EZCFG_LXC_CONF_KEYWORD_LXC_DEVTTYDIR
#define EZCFG_LXC_CONF_1_LXC_MOUNT          "c1." EZCFG_LXC_CONF_KEYWORD_LXC_MOUNT
#define EZCFG_LXC_CONF_1_LXC_MOUNT_ENTRY    "c1." EZCFG_LXC_CONF_KEYWORD_LXC_MOUNT_ENTRY
#define EZCFG_LXC_CONF_1_LXC_ROOTFS         "c1." EZCFG_LXC_CONF_KEYWORD_LXC_ROOTFS
#define EZCFG_LXC_CONF_1_LXC_ROOTFS_MOUNT   "c1." EZCFG_LXC_CONF_KEYWORD_LXC_ROOTFS_MOUNT
#define EZCFG_LXC_CONF_1_LXC_PIVOTDIR       "c1." EZCFG_LXC_CONF_KEYWORD_LXC_PIVOTDIR
#define EZCFG_LXC_CONF_1_LXC_CGROUP_CPUSET_CPUS \
	"c1." EZCFG_LXC_CONF_KEYWORD_LXC_CGROUP_CPUSET_CPUS
#define EZCFG_LXC_CONF_1_LXC_CAP_DROP       "c1." EZCFG_LXC_CONF_KEYWORD_LXC_CAP_DROP

#endif
