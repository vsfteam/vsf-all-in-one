/*****************************************************************************
 *   Copyright(C)2009-2022 by VSF Team                                       *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 ****************************************************************************/


/*============================ INCLUDES ======================================*/

#include "kernel/vsf_kernel_cfg.h"

#if VSF_USE_KERNEL == ENABLED && defined(__EDA_GADGET__)

#if __IS_COMPILER_IAR__
//! statement is unreachable
#   pragma diag_suppress=pe111
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#if VSF_CALLBACK_TIMER_CFG_SUPPORT_ISR == ENABLED
#   define __vsf_callback_timer_protect             vsf_protect(interrupt)
#   define __vsf_callback_timer_unprotect           vsf_unprotect(interrupt)
#else
#   define __vsf_callback_timer_protect             vsf_protect(scheduler)
#   define __vsf_callback_timer_unprotect           vsf_unprotect(scheduler)
#endif

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

#if     VSF_KERNEL_CFG_EDA_SUPPORT_TIMER == ENABLED
#   if VSF_SYSTIMER_CFG_IMPL_MODE != VSF_SYSTIMER_IMPL_NONE
#       if      (VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICKLESS)   \
            &&  (VSF_SYSTIMER_CFG_IMPL_MODE == VSF_SYSTIMER_IMPL_TICK_MODE)
#           error systimer is in tick mode while tickless mode is required on kernel
#       endif
#       if      (VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICK)       \
            &&  (VSF_SYSTIMER_CFG_IMPL_MODE != VSF_SYSTIMER_IMPL_TICK_MODE)
#           warning systimer is not in tick mode while tick mode is required on kernel,\
                ignore this warning if your arch does not support tick mode systimer,\
                or set VSF_SYSTIMER_CFG_IMPL_MODE to VSF_SYSTIMER_IMPL_TICK_MODE\
                for better optimization.
#       endif
#   endif

#   if      VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICKLESS     \
        &&  (VSF_SYSTIMER_CFG_IMPL_MODE == VSF_SYSTIMER_IMPL_NONE)
extern bool vsf_systimer_is_due(vsf_systimer_tick_t due);
extern vsf_err_t vsf_systimer_start(void);
extern vsf_systimer_tick_t vsf_systimer_get(void);
extern uint_fast32_t vsf_systimer_tick_to_ms(vsf_systimer_tick_t tick);
extern uint_fast32_t vsf_systimer_tick_to_us(vsf_systimer_tick_t tick);
extern vsf_systimer_tick_t vsf_systimer_ms_to_tick(uint_fast32_t time_ms);
extern vsf_systimer_tick_t vsf_systimer_us_to_tick(uint_fast32_t time_us);
#   endif

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static void __vsf_teda_timer_enqueue(vsf_teda_t *this_ptr, vsf_systimer_tick_t due);

VSF_CAL_SECTION(".text.vsf.kernel.__vsf_teda_cancel_timer")
extern vsf_err_t __vsf_teda_cancel_timer(vsf_teda_t *this_ptr);
#endif

/*============================ IMPLEMENTATION ================================*/

#if VSF_KERNEL_CFG_EDA_SUPPORT_TIMER == ENABLED
#if VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICKLESS

static void __vsf_systimer_wakeup(void)
{
    if (!__vsf_eda.timer.processing) {
        __vsf_eda.timer.processing = true;
        if (vsf_eda_post_evt((vsf_eda_t *)&__vsf_eda.task, VSF_EVT_TIMER)) {
            __vsf_eda.timer.processing = false;
        }
    }
}

#if VSF_SYSTIMER_CFG_IMPL_MODE != VSF_SYSTIMER_IMPL_NONE
static void __vsf_systimer_update(bool force)
{
    vsf_teda_t *teda;

    // TODO: need protect here?
    vsf_timq_peek(&__vsf_eda.timer.timq, teda);
    if (NULL == teda) {
        vsf_systimer_set_idle();
    } else if (force || (teda->due != __vsf_eda.timer.pre_tick)) {
        __vsf_eda.timer.pre_tick = teda->due;
        if (!vsf_systimer_set(teda->due)) {
            __vsf_systimer_wakeup();
        }
    }
}
#else
static void __vsf_systimer_update(bool force)
{

}
#endif

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static bool __vsf_systimer_is_due(vsf_systimer_tick_t due)
{
    return vsf_systimer_is_due(due);
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static void __vsf_systimer_start(void)
{
    vsf_systimer_prio_set(__vsf_eda.timer.arch_prio);

    vsf_systimer_start();
}

// DO NOT add section on vsf_systimer_evthandler, it's a weak function in arch
void vsf_systimer_evthandler(vsf_systimer_tick_t tick)
{
#if VSF_KERNEL_CFG_TRACE == ENABLED
    if (!__vsf_eda.timer.is_isr_info_sent) {
        __vsf_eda.timer.is_isr_info_sent = true;
        vsf_kernel_trace_isr_info(vsf_get_interrupt_id(), "systimer");
    }
    vsf_kernel_trace_isr_enter(vsf_get_interrupt_id());
#endif

    VSF_UNUSED_PARAM(tick);
    __vsf_systimer_wakeup();

#if VSF_KERNEL_CFG_TRACE == ENABLED
    vsf_kernel_trace_isr_leave(vsf_get_interrupt_id());
#endif
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static vsf_err_t __vsf_teda_set_timer_imp(vsf_teda_t *this_ptr, vsf_systimer_tick_t due)
{
    __vsf_teda_timer_enqueue(this_ptr, due);
    __vsf_systimer_update(false);
    return VSF_ERR_NONE;
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
vsf_systimer_tick_t vsf_systimer_get_tick(void)
{
    return vsf_systimer_get();
}
#else       // VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICKLESS
VSF_CAL_SECTION(".text.vsf.kernel.teda")
static void __vsf_systimer_start(void)
{
    // in tick timer mode, user will initialize timer and call vsf_systimer_on_tick
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static bool __vsf_systimer_is_due(vsf_systimer_tick_t due)
{
    return ((vsf_systimer_tick_signed_t)(vsf_systimer_get_tick() - due) >= 0);
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
void vsf_systimer_on_tick(void)
{
    vsf_eda_post_evt((vsf_eda_t *)&__vsf_eda.task, VSF_EVT_TIMER);
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static vsf_err_t __vsf_teda_set_timer_imp(vsf_teda_t *this_ptr, vsf_systimer_tick_t due)
{
    __vsf_teda_timer_enqueue(this_ptr, due);
    return VSF_ERR_NONE;
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
vsf_systimer_tick_t vsf_systimer_get_tick(void)
{
    return vsf_systimer_get();
}
#endif      // VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICKLESS

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static void __vsf_systimer_init(void)
{
#if VSF_KERNEL_CFG_TIMER_MODE == VSF_KERNEL_CFG_TIMER_MODE_TICKLESS
    __vsf_eda.timer.processing = false;
#endif
    vsf_timq_init(&__vsf_eda.timer.timq);
#if VSF_KERNEL_CFG_SUPPORT_CALLBACK_TIMER == ENABLED
    vsf_callback_timq_init(&__vsf_eda.timer.callback_timq);
    vsf_callback_timq_init(&__vsf_eda.timer.callback_timq_done);
#endif

#if VSF_SYSTIMER_CFG_IMPL_MODE != VSF_SYSTIMER_IMPL_NONE
    vsf_systimer_init();
#endif
}

VSF_CAL_SECTION(".text.vsf.kernel.teda")
static void __vsf_teda_timer_enqueue(vsf_teda_t *this_ptr, vsf_systimer_tick_t due)
{
    VSF_KERNEL_ASSERT((this_ptr != NULL) && !this_ptr->use_as__vsf_eda_t.flag.state.is_timed);
    this_ptr->due = due;

    vsf_timq_insert(&__vsf_eda.timer.timq, this_ptr);
    this_ptr->use_as__vsf_eda_t.flag.state.is_timed = true;
#if VSF_KERNEL_OPT_AVOID_UNNECESSARY_YIELD_EVT == ENABLED
    vsf_protect_t origlevel = vsf_protect_int();
        this_ptr->use_as__vsf_eda_t.is_evt_incoming = true;
    vsf_unprotect_int(origlevel);
#endif
}

#if VSF_KERNEL_CFG_SUPPORT_CALLBACK_TIMER == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_callback_timer_init")
void vsf_callback_timer_init(vsf_callback_timer_t *timer)
{
    timer->due = 0;
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_callback_timer_add")
vsf_err_t vsf_callback_timer_add(vsf_callback_timer_t *timer, vsf_systimer_tick_t tick)
{
    vsf_protect_t origlevel;
#   if VSF_CALLBACK_TIMER_CFG_SUPPORT_ISR == ENABLED
    vsf_protect_t lock_status;
#   endif
    bool is_to_update = false;
    VSF_KERNEL_ASSERT(timer != NULL);

    if (timer->due != 0) {
        vsf_kernel_err_report(VSF_KERNEL_ERR_INVALID_USAGE);
        return VSF_ERR_FAIL;
    }

    origlevel = vsf_protect_sched();
#   if VSF_CALLBACK_TIMER_CFG_SUPPORT_ISR == ENABLED
    lock_status = __vsf_callback_timer_protect();
#   endif
        timer->due = tick + vsf_systimer_get_tick();
        vsf_callback_timq_insert(&__vsf_eda.timer.callback_timq, timer);
        if (NULL == timer->timer_node.prev) {
            is_to_update = true;
        }
#   if VSF_CALLBACK_TIMER_CFG_SUPPORT_ISR == ENABLED
    __vsf_callback_timer_unprotect(lock_status);
#   endif
    if (is_to_update) {
        __vsf_teda_cancel_timer((vsf_teda_t *)&__vsf_eda.task);
        __vsf_teda_set_timer_imp((vsf_teda_t *)&__vsf_eda.task, timer->due);
    }
    vsf_unprotect_sched(origlevel);
    return VSF_ERR_NONE;
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_callback_timer_remove")
vsf_err_t vsf_callback_timer_remove(vsf_callback_timer_t *timer)
{
    vsf_protect_t lock_status;
    VSF_KERNEL_ASSERT(timer != NULL);

    lock_status = __vsf_callback_timer_protect();
        if (timer->due != 0) {
            timer->due = 0;
            vsf_callback_timq_remove(&__vsf_eda.timer.callback_timq, timer);
        }
    __vsf_callback_timer_unprotect(lock_status);
    return VSF_ERR_NONE;
}

#    if VSF_CALLBACK_TIMER_CFG_SUPPORT_ISR == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_callback_timer_add_isr")
vsf_err_t vsf_callback_timer_add_isr(vsf_callback_timer_t *timer, vsf_systimer_tick_t tick)
{
    VSF_KERNEL_ASSERT(timer != NULL);
    if (timer->due != 0) {
        vsf_kernel_err_report(VSF_KERNEL_ERR_INVALID_USAGE);
        return VSF_ERR_FAIL;
    }

    timer->due = tick + vsf_systimer_get_tick();
    return vsf_eda_post_evt_msg((vsf_eda_t *)&__vsf_eda.task, VSF_KERNEL_EVT_CALLBACK_TIMER_ADD, timer);
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_callback_timer_remove_isr")
vsf_err_t vsf_callback_timer_remove_isr(vsf_callback_timer_t *timer)
{
    return vsf_callback_timer_remove(timer);
}
#    endif  // VSF_CALLBACK_TIMER_CFG_SUPPORT_ISR
#endif      // VSF_KERNEL_CFG_SUPPORT_CALLBACK_TIMER

VSF_CAL_SECTION(".text.vsf.kernel.vsf_systimer_get_duration")
vsf_systimer_tick_t vsf_systimer_get_duration(vsf_systimer_tick_t from_time, vsf_systimer_tick_t to_time)
{
    if (to_time >= from_time) {
        return (vsf_systimer_tick_t)(to_time - from_time);
    } else {
        return (vsf_systimer_tick_t)(to_time + 0xFFFFFFFF - from_time);
    }
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_systimer_get_elapsed")
vsf_systimer_tick_t vsf_systimer_get_elapsed(vsf_systimer_tick_t from_time)
{
    return vsf_systimer_get_duration(from_time, vsf_systimer_get_tick());
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_teda_start")
vsf_err_t vsf_teda_start(vsf_teda_t *this_ptr, vsf_eda_cfg_t *cfg)
{
    VSF_KERNEL_ASSERT(this_ptr != NULL);
    return vsf_eda_start(&(this_ptr->use_as__vsf_eda_t), cfg);
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_teda_set_due_ex")
vsf_err_t vsf_teda_set_due_ex(vsf_teda_t *this_ptr, vsf_systimer_tick_t due)
{
    vsf_protect_t origlevel;
    vsf_err_t err;

    origlevel = vsf_protect_sched();
        err = __vsf_teda_set_timer_imp(this_ptr, due);
    vsf_unprotect_sched(origlevel);
    return err;
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_teda_set_timer_ex")
vsf_err_t vsf_teda_set_timer_ex(vsf_teda_t *this_ptr, vsf_systimer_tick_t tick)
{
    if (0 == tick) {
        VSF_KERNEL_ASSERT(false);
        return VSF_ERR_NOT_AVAILABLE;
    }
    return vsf_teda_set_due_ex(this_ptr, vsf_systimer_get_tick() + tick);
}

#if __IS_COMPILER_ARM_COMPILER_6__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wcast-align"
#endif

#if __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wcast-align"
#endif

VSF_CAL_SECTION(".text.vsf.kernel.vsf_teda_set_timer")
vsf_err_t vsf_teda_set_timer(vsf_systimer_tick_t tick)
{
    vsf_teda_t *teda = (vsf_teda_t *)vsf_eda_get_cur();
    VSF_KERNEL_ASSERT(teda != NULL);
    return vsf_teda_set_timer_ex(teda, tick);
}

#if __IS_COMPILER_ARM_COMPILER_6__
#   pragma clang diagnostic pop
#endif

#if __IS_COMPILER_GCC__
#   pragma GCC diagnostic pop
#endif

#if __IS_COMPILER_ARM_COMPILER_6__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wcast-align"
#endif

#if __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wcast-align"
#endif

VSF_CAL_SECTION(".text.vsf.kernel.__vsf_teda_cancel_timer")
vsf_err_t __vsf_teda_cancel_timer(vsf_teda_t *this_ptr)
{
    vsf_protect_t lock_status;
    this_ptr = (vsf_teda_t *)__vsf_eda_get_valid_eda((vsf_eda_t *)this_ptr);

    VSF_KERNEL_ASSERT(this_ptr != NULL);

    lock_status = vsf_protect_sched();
        if (this_ptr->use_as__vsf_eda_t.flag.state.is_timed) {
            vsf_timq_remove(&__vsf_eda.timer.timq, this_ptr);
            this_ptr->use_as__vsf_eda_t.flag.state.is_timed = false;
        }
        this_ptr->use_as__vsf_eda_t.flag.state.is_to_set_due = false;
    vsf_unprotect_sched(lock_status);
    return VSF_ERR_NONE;
}

VSF_CAL_SECTION(".text.vsf.kernel.vsf_teda_cancel_timer")
vsf_err_t vsf_teda_cancel_timer(void)
{
    __vsf_teda_cancel_timer(NULL);
#if VSF_KERNEL_CFG_ALLOW_KERNEL_BEING_PREEMPTED == ENABLED
    vsf_evtq_clean_evt(VSF_EVT_TIMER);
#endif
    return VSF_ERR_NONE;
}

#if __IS_COMPILER_ARM_COMPILER_6__
#   pragma clang diagnostic pop
#endif

#if __IS_COMPILER_GCC__
#   pragma GCC diagnostic pop
#endif

#endif      // VSF_KERNEL_CFG_EDA_SUPPORT_TIMER

#if __IS_COMPILER_IAR__
//! statement is unreachable
#   pragma diag_warning=pe111
#endif

#endif      // VSF_USE_KERNEL && __EDA_GADGET__
