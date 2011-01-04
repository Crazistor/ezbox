#ifndef _WORLD_H_
#define _WORLD_H_

#include <stdint.h>
#include <8051.h>

/* assuming system clock frequency of 12MHz, 1 step for 1us */
/* timer0 set to mode 1, 50ms interrupt */
#define W_TIMER0_COUNT	0x3cb0 /* 15536 (= 65536 - 50000) , 50ms */
#define W_HZ		20

#define W_P1_0		P1_0
#define W_P1_1		P1_1
#define W_P1_2		P1_2
#define W_P1_3		P1_3

/* mapping to 8051 PCON register */
#define W_PCON		PCON
#define W_PCON_IDL	0x01
#define W_PCON_PD	0x02

#define W_THREAD_MAX	8
#define W_THREAD_NUM	4

#if (W_THREAD_MAX < W_THREAD_NUM)
#error "W_THREAD_NUM should be less than or equal to W_THREAD_MAX"
#endif

/* + 4 is for thread_context_switch static variables */
#define W_DATA_SIZE	(sizeof(wd) + sizeof(bp) + 4)
#define W_IRAM_SIZE	0x80
#define W_SP_BOTTOM	0x30
#define W_SP_TOP	(W_IRAM_SIZE - W_DATA_SIZE)
#define W_SP		SP

/* for critical area */
#define W_EA		EA

#define W_INIT_CRITICAL() \
do { \
	wd.crit_sum = 0; \
	/* W_EA = 1;*/ \
} while(0)

#define W_LOCK_CRITICAL() \
do { \
	W_EA = 0; \
	wd.crit_sum++; \
} while(0)

#define W_UNLOCK_CRITICAL() \
do { \
	wd.crit_sum--; \
	if (wd.crit_sum == 0) W_EA = 1; \
} while(0)


/* for interrupt handler */
#define W_INIT_INTERRUPT() \
do { \
	wd.int_sum = 0; \
	/* W_EA = 1;*/ \
} while(0)

#define W_ENTER_INTERRUPT() \
do { \
	wd.int_sum++; \
	W_EA = 0; \
	wd.crit_sum++; \
} while(0)

#define W_EXIT_INTERRUPT() \
do { \
	wd.crit_sum--; \
	if (wd.crit_sum == 0) W_EA = 1; \
	wd.int_sum--; \
	if (wd.int_sum == 0) w_thread_schedule(); \
} while(0)

typedef struct world_data_s {
	uint8_t	wid;
	uint8_t crit_sum;		/* critical area entrances */
	uint8_t int_sum;		/* interrupted times */
	uint8_t time_ticks;		/* time ticks since last update
	                    		   of world_uptime_seconds */
	uint32_t uptime_seconds;	/* seconds since world starts */
	uint8_t cur_thread_id;		/* current running thread ID */
	uint8_t next_thread_id;		/* next will run thread ID */
	uint8_t thread_tts[W_THREAD_NUM];	/* thread time to start in ticks */
	__idata volatile uint8_t * thread_spb[W_THREAD_NUM+1];	/* thread stack pointer base */
} world_data_t;

typedef struct world_rules_s {
	__code void (* space_init)(void) __reentrant;
	__code void (* time_init)(void) __reentrant;
	__code void (* threads[W_THREAD_NUM])(void) __reentrant;
	__code void (* thread_init)(void) __reentrant;
	__code void (* thread_context_switch)(void) __reentrant;
	__code void (* thread_schedule)(void) __reentrant;
	__code void (* startup)(void) __reentrant;
} world_rules_t;

/* global variables */
extern __data unsigned char bp;

/* global functions */
extern __code void w_space_init(void) __using 2;
extern __code void w_time_init(void) __using 2;
extern __code void (* __code w_threads[W_THREAD_NUM])(void);
extern __code void w_thread_init(void) __using 2;
extern __code void w_thread_context_switch(void) __using 2;
extern __code void w_thread_schedule(void) __using 2;
extern __code void w_startup(void) __using 2;
extern __code void world(void) __using 2;

extern __code void w_thread_wait(uint8_t ticks) __using 2;
#endif
