/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : ezcfg-rp_pppoe.h
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2008-2013 by ezbox-project
 *
 * History      Rev       Description
 * 2012-09-23   0.1       Write it from scratch
 * ============================================================================
 */

#ifndef _EZCFG_RP_PPPOE_H_
#define _EZCFG_RP_PPPOE_H_

/* ezcfg nvram name prefix */
#define EZCFG_RP_PPPOE_NVRAM_PREFIX     "rp_pppoe."

/* PPP general options */
#define EZCFG_RP_PPPOE_PPP_DEBUG        "ppp_debug"
#define EZCFG_RP_PPPOE_PPP_KDEBUG       "ppp_kdebug"
#define EZCFG_RP_PPPOE_PPP_NODETACH     "ppp_nodetach"
#define EZCFG_RP_PPPOE_PPP_UPDETACH     "ppp_updetach"
#define EZCFG_RP_PPPOE_PPP_HOLDOFF      "ppp_holdoff"
#define EZCFG_RP_PPPOE_PPP_IDLE         "ppp_idle"
#define EZCFG_RP_PPPOE_PPP_MAXCONNECT   "ppp_maxconnect"
#define EZCFG_RP_PPPOE_PPP_DOMAIN       "ppp_domain"
#define EZCFG_RP_PPPOE_PPP_PERSIST      "ppp_persist"
#define EZCFG_RP_PPPOE_PPP_DEMAND       "ppp_demand"
#define EZCFG_RP_PPPOE_PPP_LOGFILE      "ppp_logfile"
#define EZCFG_RP_PPPOE_PPP_LINKNAME     "ppp_linkname"
#define EZCFG_RP_PPPOE_PPP_IFNAME       "ppp_ifname"
#define EZCFG_RP_PPPOE_PPP_MAXFAIL      "ppp_maxfail"
#define EZCFG_RP_PPPOE_PPP_KTUNE        "ppp_ktune"
#define EZCFG_RP_PPPOE_PPP_CONNECT_DELAY "ppp_connect_delay"
#define EZCFG_RP_PPPOE_PPP_UNIT         "ppp_unit"
#define EZCFG_RP_PPPOE_PPP_DUMP         "ppp_dump"
#define EZCFG_RP_PPPOE_PPP_DRYRUN       "ppp_dryrun"
#define EZCFG_RP_PPPOE_PPP_CHILD_TIMEOUT "ppp_child_timeout"
#define EZCFG_RP_PPPOE_PPP_IP_UP_SCRIPT "ppp_ip_up_script"
#define EZCFG_RP_PPPOE_PPP_IP_DOWN_SCRIPT "ppp_ip_down_script"
#define EZCFG_RP_PPPOE_PPP_IPV6_UP_SCRIPT "ppp_ipv6_up_script"
#define EZCFG_RP_PPPOE_PPP_IPV6_DOWN_SCRIPT "ppp_ipv6_down_script"
#define EZCFG_RP_PPPOE_PPP_PLUGIN       "ppp_plugin"

/* PPP auth options */
#define EZCFG_RP_PPPOE_PPP_AUTH         "ppp_auth"
#define EZCFG_RP_PPPOE_PPP_REQUIRE_PAP  "ppp_require_pap"
#define EZCFG_RP_PPPOE_PPP_REQUIRE_CHAP "ppp_require_chap"
#define EZCFG_RP_PPPOE_PPP_REQUIRE_MSCHAP "ppp_require_ms_chap"
#define EZCFG_RP_PPPOE_PPP_REQUIRE_MSCHAP_V2 "ppp_require_ms_chap_v2"
#define EZCFG_RP_PPPOE_PPP_REFUSE_PAP   "ppp_refuse_pap"
#define EZCFG_RP_PPPOE_PPP_REFUSE_CHAP  "ppp_refuse_chap"
#define EZCFG_RP_PPPOE_PPP_REFUSE_MSCHAP "ppp_refuse_ms_chap"
#define EZCFG_RP_PPPOE_PPP_REFUSE_MSCHAP_V2 "ppp_refuse_ms_chap_v2"
#define EZCFG_RP_PPPOE_PPP_REQUIRE_EAP  "ppp_require_eap"
#define EZCFG_RP_PPPOE_PPP_REFUSE_EAP   "ppp_refuse_eap"
#define EZCFG_RP_PPPOE_PPP_NAME         "ppp_name"
#define EZCFG_RP_PPPOE_PPP_USER         "ppp_user"
#define EZCFG_RP_PPPOE_PPP_PASSWORD     "ppp_password"
#define EZCFG_RP_PPPOE_PPP_USEHOSTNAME  "ppp_usehostname"
#define EZCFG_RP_PPPOE_PPP_REMOTENAME   "ppp_remotename"
#define EZCFG_RP_PPPOE_PPP_LOGIN        "ppp_login"
#define EZCFG_RP_PPPOE_PPP_ENABLE_SESSION "ppp_enable_session"
#define EZCFG_RP_PPPOE_PPP_PAPCRYPT     "ppp_papcrypt"
#define EZCFG_RP_PPPOE_PPP_PRIVGROUP    "ppp_privgroup"
#define EZCFG_RP_PPPOE_PPP_ALLOW_IP     "ppp_allow_ip"
#define EZCFG_RP_PPPOE_PPP_REMOTENUMBER "ppp_remotenumber"
#define EZCFG_RP_PPPOE_PPP_ALLOW_NUMBER "ppp_allow_number"

/* PPP LCP options */
#define EZCFG_RP_PPPOE_PPP_LCP_NOOPT    "ppp_lcp_noopt"
#define EZCFG_RP_PPPOE_PPP_NOACCOMP     "ppp_noaccomp"
#define EZCFG_RP_PPPOE_PPP_NOMAGIC      "ppp_nomagic"
#define EZCFG_RP_PPPOE_PPP_MRU          "ppp_mru"
#define EZCFG_RP_PPPOE_PPP_MTU          "ppp_mtu"
#define EZCFG_RP_PPPOE_PPP_NOPCOMP      "ppp_nopcomp"
#define EZCFG_RP_PPPOE_PPP_PASSIVE      "ppp_passive"
#define EZCFG_RP_PPPOE_PPP_SILENT       "ppp_silent"
#define EZCFG_RP_PPPOE_PPP_LCP_ECHO_FAILURE "ppp_lcp_echo_failure"
#define EZCFG_RP_PPPOE_PPP_LCP_ECHO_INTERVAL "ppp_lcp_echo_interval"
#define EZCFG_RP_PPPOE_PPP_LCP_RESTART  "ppp_lcp_restart"
#define EZCFG_RP_PPPOE_PPP_LCP_MAX_TERMINATE "ppp_lcp_max_terminate"
#define EZCFG_RP_PPPOE_PPP_LCP_MAX_CONFIGURE "ppp_lcp_max_configure"
#define EZCFG_RP_PPPOE_PPP_LCP_MAX_FAILURE "ppp_lcp_max_failure"
#define EZCFG_RP_PPPOE_PPP_RECEIVE_ALL  "ppp_receive_all"
#define EZCFG_RP_PPPOE_PPP_NOENDPOINT   "ppp_noendpoint"

/* PPP IPCP options */
#define EZCFG_RP_PPPOE_PPP_NOIP         "ppp_noip"
#define EZCFG_RP_PPPOE_PPP_NOVJ         "ppp_novj"
#define EZCFG_RP_PPPOE_PPP_NOVJCCOMP    "ppp_novjccomp"
#define EZCFG_RP_PPPOE_PPP_VJ_MAX_SLOTS "ppp_vj_max_slots"
#define EZCFG_RP_PPPOE_PPP_IPCP_ACCEPT_LOCAL "ppp_ipcp_accept_local"
#define EZCFG_RP_PPPOE_PPP_IPCP_ACCEPT_REMOTE "ppp_ipcp_accept_remote"
#define EZCFG_RP_PPPOE_PPP_IPPARAM      "ppp_ipparam"
#define EZCFG_RP_PPPOE_PPP_NOIPDEFAULT  "ppp_noipdefault"
#define EZCFG_RP_PPPOE_PPP_MS_DNS       "ppp_ms_dns"
#define EZCFG_RP_PPPOE_PPP_MS_WINS      "ppp_ms_wins"
#define EZCFG_RP_PPPOE_PPP_IPCP_RESTART "ppp_ipcp_restart"
#define EZCFG_RP_PPPOE_PPP_IPCP_MAX_TERMINATE "ppp_ipcp_max_terminate"
#define EZCFG_RP_PPPOE_PPP_IPCP_MAX_CONFIGURE "ppp_ipcp_max_configure"
#define EZCFG_RP_PPPOE_PPP_IPCP_MAX_FAILURE   "ppp_ipcp_max_failure"
#define EZCFG_RP_PPPOE_PPP_DEFAULTROUTE "ppp_defaultroute"
#define EZCFG_RP_PPPOE_PPP_REPLACEDEFAULTROUTE "ppp_replacedefaultroute"
#define EZCFG_RP_PPPOE_PPP_PROXYARP     "ppp_proxyarp"
#define EZCFG_RP_PPPOE_PPP_USEPEERDNS   "ppp_usepeerdns"
#define EZCFG_RP_PPPOE_PPP_NETMASK      "ppp_netmask"
#define EZCFG_RP_PPPOE_PPP_IPCP_NO_ADDRESSES "ppp_ipcp_no_addresses"
#define EZCFG_RP_PPPOE_PPP_IPCP_NO_ADDRESS   "ppp_ipcp_no_address"
#define EZCFG_RP_PPPOE_PPP_NOREMOTEIP   "ppp_noremoteip"
#define EZCFG_RP_PPPOE_PPP_NOSENDIP     "ppp_nosendip"
#define EZCFG_RP_PPPOE_PPP_IP_ADDRESSES "ppp_ip_addresses"

/* PPP tty options */
#define EZCFG_RP_PPPOE_PPP_LOCK         "ppp_lock"

/* PPP rp-pppoe plugin options */
#define EZCFG_RP_PPPOE_DEVICE_NAME      "device_name"
#define EZCFG_RP_PPPOE_RP_PPPOE_SERVICE "rp_pppoe_service"
#define EZCFG_RP_PPPOE_RP_PPPOE_AC      "rp_pppoe_ac"
#define EZCFG_RP_PPPOE_RP_PPPOE_SESS    "rp_pppoe_sess"
#define EZCFG_RP_PPPOE_RP_PPPOE_VERBOSE "rp_pppoe_verbose"
#define EZCFG_RP_PPPOE_PPPOE_MAC        "pppoe_mac"

#endif
