/*
 * kernel.h
 *
 *  Created on: 2010-3-8
 *      Author: jivin
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include <autocfg.h>
#include <err.h>
#include <core.h>
#include <cpu.h>
#include <thread.h>
#include <lsched.h>
#include <int.h>
#include <timer.h>
#include <mem.h>
#include <resource.h>
#include <event.h>
#include <mutex.h>

#ifdef CFG_EVT_SEM
#include <sem.h>
#endif
#ifdef CFG_EVT_MBOX
#include <mbox.h>
#endif

#include <policy.h>
#include <comm_thrd.h>
#ifdef CFG_THRD_SLICE
#include <slice_thrd.h>
#endif

#ifdef CFG_THRD_PERIOD
#include <period_thrd.h>
#endif

#ifdef CFG_THRD_POSIX
#include <posix_thrd.h>
#endif

#ifdef CFG_THRD_RM
#include <rm_thrd.h>
#endif

#ifdef CFG_CMP
#include <ipi.h>
#include <cmp.h>
#endif

#include <atomic.h>
#include <spinlock.h>
#endif /* KERNEL_H_ */
