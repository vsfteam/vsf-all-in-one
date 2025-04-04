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

#ifndef __VSF_THREAD_H__
#define __VSF_THREAD_H__

/*============================ INCLUDES ======================================*/

#include "kernel/vsf_kernel_cfg.h"

#if VSF_KERNEL_CFG_SUPPORT_THREAD == ENABLED && VSF_USE_KERNEL == ENABLED
#include "../vsf_eda.h"

#if VSF_KERNEL_CFG_EDA_SUPPORT_TASK == ENABLED
#   include "./vsf_task.h"
#endif

#if     defined(__VSF_THREAD_CLASS_IMPLEMENT)
#   undef __VSF_THREAD_CLASS_IMPLEMENT
#   define __PLOOC_CLASS_IMPLEMENT__
#elif   defined(__VSF_THREAD_CLASS_INHERIT__)
#   undef __VSF_THREAD_CLASS_INHERIT__
#   define __PLOOC_CLASS_INHERIT__
#endif

#include "utilities/ooc_class.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/

#ifndef VSF_KERNEL_CFG_THREAD_STACK_PAGE_SIZE
#   define VSF_KERNEL_CFG_THREAD_STACK_PAGE_SIZE        1
#endif
#ifndef VSF_KERNEL_CFG_THREAD_STACK_GUARDIAN_SIZE
#   define VSF_KERNEL_CFG_THREAD_STACK_GUARDIAN_SIZE    0
#endif

#ifndef VSF_KERNEL_CFG_THREAD_STACK_ALIGN_BIT
#   ifdef VSF_ARCH_STACK_ALIGN_BIT
#       define VSF_KERNEL_CFG_THREAD_STACK_ALIGN_BIT    VSF_ARCH_STACK_ALIGN_BIT
#   else
#       define VSF_KERNEL_CFG_THREAD_STACK_ALIGN_BIT    3
#   endif
#endif

#define __VSF_THREAD_STACK_SAFE_SIZE(__stack)                                   \
            (   (   (   ((__stack) + VSF_KERNEL_CFG_THREAD_STACK_PAGE_SIZE - 1) \
                    /   VSF_KERNEL_CFG_THREAD_STACK_PAGE_SIZE)                  \
                * VSF_KERNEL_CFG_THREAD_STACK_PAGE_SIZE)                        \
            + VSF_KERNEL_CFG_THREAD_STACK_GUARDIAN_SIZE)

#ifdef VSF_ARCH_LIMIT_NO_SET_STACK
// arch does not support set stack, use host_thread mode
#   define VSF_KERNEL_THREAD_USE_HOST                   ENABLED
#   ifdef VSF_ARCH_RTOS_DYNAMIC_STACK
#       define VSF_KERNEL_THREAD_DYNAMIC_STACK          ENABLED
#       if VSF_KERNEL_CFG_THREAD_STACK_CHECK == ENABLED
#           warning VSF_KERNEL_CFG_THREAD_STACK_CHECK is not usable for dynamic stack
#           undef VSF_KERNEL_CFG_THREAD_STACK_CHECK
#           define VSF_KERNEL_CFG_THREAD_STACK_CHECK    DISABLED
#       endif
#   endif
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/

#define __declare_vsf_thread(__name)                                            \
            typedef struct __name __name;                                       \
            typedef struct thread_cb_##__name##_t thread_cb_##__name##_t;
#define declare_vsf_thread(__name)      __declare_vsf_thread(__name)
#define declare_vsf_thread_ex(__name)   __declare_vsf_thread(__name)

#define dcl_vsf_thread(__name)          declare_vsf_thread(__name)
#define dcl_vsf_thread_ex(__name)       declare_vsf_thread(__name)

#if VSF_KERNEL_THREAD_DYNAMIC_STACK == ENABLED
#   define __vsf_thread_set_stack_canery(__thread, __task)                      \
            (__thread)->canary = 0xDEADBEEF;
#else
#   define __vsf_thread_set_stack_canery(__thread, __task)                      \
            (__task)->canary = 0xDEADBEEF;
#endif

#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
#   define __vsf_thread_set_stack(__thread, __task, __stack_ptr, __stack_bytesize)\
            (__thread)->use_as__vsf_thread_cb_t.stack = (__stack_ptr);          \
            (__thread)->use_as__vsf_thread_cb_t.stack_size = (__stack_bytesize);\
            __vsf_thread_set_stack_canery(__thread, __task)
#else
#   define __vsf_thread_set_stack(__thread, __task, __stack_ptr, __stack_bytesize)\
            (__thread)->stack = (__stack_ptr);                                  \
            (__thread)->stack_size = (__stack_bytesize);                        \
            __vsf_thread_set_stack_canery(__thread, __task)
#endif

#if VSF_KERNEL_THREAD_DYNAMIC_STACK == ENABLED
#   define __vsf_thread_def_stack(__name, __bytesize)                           \
            typedef enum {                                                      \
                vsf_thread##__name##_stack_bytesize = (__bytesize),             \
            };
#   define __vsf_thread_def_stack_member(__name, __bytesize)
#   define __vsf_thread_imp_stack(__name, __thread, __task)                     \
            __vsf_thread_set_stack((__thread), (__task), NULL, (vsf_thread##__name##_stack_bytesize))
#   define __vsf_eda_call_thread_prepare_stack(__name, __thread)                \
            .stack = NULL,                                                      \
            .stack_size = (vsf_thread##__name##_stack_bytesize),
#else
#   define __vsf_thread_def_stack(__name, __bytesize)
#   define __vsf_thread_def_stack_member(__name, __bytesize)                    \
            uint32_t canary;                                                    \
            uint64_t stack_arr[(__VSF_THREAD_STACK_SAFE_SIZE(__bytesize) + 7) / 8]\
                        VSF_CAL_ALIGN(1 << VSF_KERNEL_CFG_THREAD_STACK_ALIGN_BIT);
#   define __vsf_thread_imp_stack(__name, __thread, __task)                     \
            __vsf_thread_set_stack((__thread), (__task), (__task)->stack_arr, sizeof((__task)->stack_arr))
#   define __vsf_eda_call_thread_prepare_stack(__name, __thread)                \
            .stack = (__thread)->stack_arr,                                     \
            .stack_size = sizeof((__thread)->stack_arr),
#endif

#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
#   define __def_vsf_thread(__name, __stack_bytesize, ...)                      \
            __vsf_thread_def_stack(__name, (__stack_bytesize))                  \
            struct thread_cb_##__name##_t {                                     \
                implement(vsf_thread_cb_t)                                      \
                __VA_ARGS__                                                     \
                __vsf_thread_def_stack_member(__name, (__stack_bytesize))       \
            };                                                                  \
            struct __name {                                                     \
                implement(vsf_thread_t)                                         \
                implement_ex(thread_cb_##__name##_t, param)                     \
            } VSF_CAL_ALIGN(8);                                                 \
            extern void vsf_thread_##__name##_start(struct __name *task,        \
                                                    vsf_prio_t priority);       \
            extern void vsf_thread_##__name##_entry(                            \
                            struct thread_cb_##__name##_t *vsf_pthis);

#   define __implement_vsf_thread(__name)                                       \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis);              \
            void vsf_thread_##__name##_start( struct __name *task,              \
                                                vsf_prio_t priority)            \
            {                                                                   \
                VSF_KERNEL_ASSERT(NULL != task);                                \
                thread_cb_##__name##_t *__vsf_pthis = &(task->param);           \
                __vsf_pthis->use_as__vsf_thread_cb_t.entry = (vsf_thread_entry_t *)\
                                    &vsf_thread_##__name##_entry;               \
                __vsf_thread_imp_stack(__name, __vsf_pthis, task)               \
                vsf_thread_start(   &(task->use_as__vsf_thread_t),              \
                                    &(__vsf_pthis->use_as__vsf_thread_cb_t),    \
                                    priority);                                  \
            }                                                                   \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis)

#   define __vsf_eda_call_thread_prepare(__name, __thread_cb)                   \
            do {                                                                \
                thread_cb_##__name##_t *__vsf_pthis = (__thread_cb);            \
                const vsf_thread_prepare_cfg_t cfg = {                          \
                    .entry = (vsf_thread_entry_t *)                             \
                                    &vsf_thread_##__name##_entry,               \
                    __vsf_eda_call_thread_prepare_stack(__name, (__thread_cb))  \
                };                                                              \
                vk_eda_call_thread_prepare(&(__vsf_pthis->use_as__vsf_thread_cb_t),\
                                              (vsf_thread_prepare_cfg_t *)&cfg);\
            } while(0)

#   define vsf_eda_call_thread_prepare(__name, __thread_cb)                     \
                __vsf_eda_call_thread_prepare(__name, __thread_cb)

#   define vsf_eda_call_thread(__thread_cb)                                     \
                vk_eda_call_thread(&((__thread_cb)->use_as__vsf_thread_cb_t))


#   define __def_vsf_thread_ex(__name, ...)                                     \
            struct thread_cb_##__name##_t {                                     \
                implement(vsf_thread_cb_t)                                      \
                __VA_ARGS__                                                     \
            };                                                                  \
            struct __name {                                                     \
                implement(vsf_thread_t)                                         \
                implement_ex(thread_cb_##__name##_t, param)                     \
            };                                                                  \
            extern void vsf_thread_##__name##_start( struct __name *task,       \
                                                vsf_prio_t priority,            \
                                                void *stack,                    \
                                                uint_fast32_t size);            \
            extern void vsf_thread_##__name##_entry(                            \
                            struct thread_cb_##__name##_t *vsf_pthis);


#   define __implement_vsf_thread_ex(__name)                                    \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis);              \
            void vsf_thread_##__name##_start(   struct __name *task,            \
                                                vsf_prio_t priority,            \
                                                void *stack,                    \
                                                uint_fast32_t size)             \
            {                                                                   \
                VSF_KERNEL_ASSERT(NULL != task && 0 != size && NULL != stack);  \
                thread_cb_##__name##_t *__vsf_pthis = &(task->param);           \
                __vsf_pthis->use_as__vsf_thread_cb_t.entry = (vsf_thread_entry_t *)\
                                    &vsf_thread_##__name##_entry;               \
                __vsf_pthis->use_as__vsf_thread_cb_t.stack = stack;             \
                __vsf_pthis->use_as__vsf_thread_cb_t.stack_size = size;         \
                vsf_thread_start(   &(task->use_as__vsf_thread_t),              \
                                    &(task->param.use_as__vsf_thread_cb_t),     \
                                    priority);                                  \
            }                                                                   \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis)

#   define __vsf_eda_call_thread_prepare_ex__(  __name,                         \
                                                __thread_cb,                    \
                                                __stack,                        \
                                                __stack_bytesize)               \
            do {                                                                \
                VSF_KERNEL_ASSERT((NULL != (__stack)) && (0 != (__size)))       \
                thread_cb_##__name##_t *__vsf_pthis = (__thread_cb);            \
                const vsf_thread_prepare_cfg_t cfg = {                          \
                    .entry = (vsf_thread_entry_t *)                             \
                                    &vsf_thread_##__name##_entry,               \
                    .stack = (__stack),                                         \
                    .stack_size = (__stack_bytesize),                           \
                };                                                              \
                vk_eda_call_thread_prepare(&(__vsf_pthis->use_as__vsf_thread_cb_t),\
                                              (vsf_thread_prepare_cfg_t *)&cfg);\
            } while(0)


#   define vsf_eda_call_thread_prepare_ex(  __name,                             \
                                            __thread_cb,                        \
                                            __stack,                            \
                                            __stack_bytesize)                   \
                __vsf_eda_call_thread_prepare_ex__( __name,                     \
                                                    (__thread_cb),              \
                                                    (__stack),                  \
                                                    (__stack_bytesize))

#   define vsf_eda_call_thread_ex(__thread_cb)                                  \
                vk_eda_call_thread(&((__thread_cb)->use_as__vsf_thread_cb_t))

#else
#   define __def_vsf_thread(__name, __stack_bytesize, ...)                      \
            struct thread_cb_##__name##_t {                                     \
                implement(vsf_thread_t)                                         \
                __VA_ARGS__                                                     \
            };                                                                  \
            __vsf_thread_def_stack(__name, (__stack_bytesize))                  \
            struct __name {                                                     \
                __vsf_thread_def_stack_member(__name, (__stack_bytesize))       \
                implement_ex(thread_cb_##__name##_t, param);                    \
            } VSF_CAL_ALIGN(8);                                                 \
            extern void vsf_thread_##__name##_start(struct __name *task,        \
                                                    vsf_prio_t priority);       \
            extern void vsf_thread_##__name##_entry(                            \
                        struct thread_cb_##__name##_t *vsf_pthis);

#   define __implement_vsf_thread(__name)                                       \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis);              \
            void vsf_thread_##__name##_start( struct __name *task,              \
                                                vsf_prio_t priority)            \
            {                                                                   \
                VSF_KERNEL_ASSERT(NULL != task);                                \
                vsf_thread_t *__vsf_pthis =                                     \
                    &(task->param.use_as__vsf_thread_t);                        \
                __vsf_pthis->entry = (vsf_thread_entry_t *)                     \
                                    &vsf_thread_##__name##_entry;               \
                __vsf_thread_imp_stack(__name, __vsf_pthis, task)               \
                vsf_thread_start(__vsf_pthis, priority);                        \
            }                                                                   \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis)

#   define __def_vsf_thread_ex(__name, ...)                                     \
            struct thread_cb_##__name##_t {                                     \
                implement(vsf_thread_t)                                         \
                __VA_ARGS__                                                     \
            };                                                                  \
            struct __name {                                                     \
                implement_ex(thread_cb_##__name##_t, param);                    \
            } VSF_CAL_ALIGN(8);                                                 \
            extern void vsf_thread_##__name##_start(struct __name *task,        \
                                                    vsf_prio_t priority);       \
            extern void vsf_thread_##__name##_entry(                            \
                        struct thread_cb_##__name##_t *vsf_pthis);


#   define __implement_vsf_thread_ex(__name)                                    \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis);              \
            void vsf_thread_##__name##_start(   struct __name *task,            \
                                                vsf_prio_t priority,            \
                                                void *stack,                    \
                                                uint_fast32_t size)             \
            {                                                                   \
                VSF_KERNEL_ASSERT(NULL != task && 0 != size && NULL != stack);  \
                vsf_thread_t *__vsf_pthis =                                     \
                    &(task->param.use_as__vsf_thread_t);                        \
                __vsf_pthis->entry = (vsf_thread_entry_t *)                     \
                                    &vsf_thread_##__name##_entry;               \
                __vsf_pthis->stack = stack;                                     \
                __vsf_pthis->stack_size = size;                                 \
                vsf_thread_start(__vsf_pthis, priority);                        \
            }                                                                   \
            void vsf_thread_##__name##_entry(                                   \
                        struct thread_cb_##__name##_t *vsf_pthis)

#endif


#define def_vsf_thread(__name, __stack_bytesize, ...)                           \
            __def_vsf_thread(__name, (__stack_bytesize), __VA_ARGS__)

#define define_vsf_thread(__name, __stack_bytesize, ...)                        \
            def_vsf_thread(__name, (__stack_bytesize), __VA_ARGS__)

#define def_vsf_thread_ex(__name, ...)                                          \
            __def_vsf_thread_ex(__name, __VA_ARGS__)

#define define_vsf_thread_ex(__name, ...)                                       \
            def_vsf_thread_ex(__name, __VA_ARGS__)

#define end_def_vsf_thread(...)
#define end_define_vsf_thread(...)

#define implement_vsf_thread(__name)        __implement_vsf_thread(__name)
#define implement_vsf_thread_ex(__name)     __implement_vsf_thread_ex(__name)

#define imp_vsf_thread(__name)              implement_vsf_thread(__name)
#define imp_vsf_thread_ex(__name)           implement_vsf_thread_ex(__name)

#define __init_vsf_thread(__name, __task, __pri)                                \
            vsf_thread_##__name##_start((__task), (__pri))

#define __init_vsf_thread_ex(__name, __task, __pri, __stack, __stack_bytesize)  \
            vsf_thread_##__name##_start((__task), (__pri), (__stack), (__stack_bytesize))

#define init_vsf_thread(__name, __task, __pri)                                  \
            __init_vsf_thread(__name, (__task), (__pri))

#define init_vsf_thread_ex(__name, __task, __pri, __stack, __stack_bytesize)    \
            __init_vsf_thread_ex(__name, (__task), (__pri), (__stack), (__stack_bytesize))

#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
#   define __vsf_thread(__name)      thread_cb_##__name##_t
#   define vsf_thread(__name)        __vsf_thread(__name)

#endif

#define vsf_thread_wfm      vsf_thread_wait_for_msg
#define vsf_thread_wfe      vsf_thread_wait_for_evt

#if VSF_KERNEL_CFG_SUPPORT_EVT_MESSAGE == ENABLED
#   define vsf_thread_wfem     vsf_thread_wait_for_evt_msg
#endif


#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
#   define __vsf_thread_call_sub(__name, __target, ...)                         \
            vk_thread_call_eda( (uintptr_t)(__name),                            \
                                (uintptr_t)(__target),                          \
                                (0, ##__VA_ARGS__),                             \
                                0,                                              \
                                0)


#   define vsf_thread_call_sub(__name, __target, ...)                           \
            __vsf_thread_call_sub(__name, (__target), (0, ##__VA_ARGS__))


#   define vsf_thread_call_pt(__name, __target, ...)                            \
            (__target)->fsm_state = 0;                                          \
            vsf_thread_call_sub(vsf_pt_func(__name), (__target), (0, ##__VA_ARGS__))

#endif

#if VSF_KERNEL_CFG_EDA_SUPPORT_TASK == ENABLED && VSF_KERNEL_CFG_EDA_SUBCALL_HAS_RETURN_VALUE == ENABLED
#   define vsf_thread_call_task(__name, __target, ...)                          \
                vk_thread_call_task(vsf_task_func(__name), __target, (0, ##__VA_ARGS__))
#endif

#if VSF_KERNEL_CFG_EDA_SUPPORT_TIMER == ENABLED
#   define vsf_thread_delay_ms(__ms)                vsf_thread_delay(vsf_systimer_ms_to_tick(__ms))
#   define vsf_thread_delay_us(__us)                vsf_thread_delay(vsf_systimer_us_to_tick(__us))
#endif

#if VSF_KERNEL_CFG_SUPPORT_SYNC == ENABLED
#   define vsf_thread_sem_post(__sem)               vsf_eda_sem_post(__sem)
#   define vsf_thread_sem_pend(__sem, timeout)      __vsf_thread_wait_for_sync(__sem, timeout)

#   define vsf_thread_trig_set(__trig, ...)         vsf_eda_trig_set(__trig, ##__VA_ARGS__)
#   define vsf_thread_trig_reset(__trig)            vsf_eda_trig_reset(__trig)
#   define vsf_thread_trig_pend(__trig, timeout)    __vsf_thread_wait_for_sync(__trig, timeout)

#   define vsf_thread_mutex_enter(__mtx, timeout)   __vsf_thread_wait_for_sync(&(__mtx)->use_as__vsf_sync_t, timeout)
#endif

/*============================ TYPES =========================================*/

declare_class(vsf_thread_t)

#   if VSF_KERNEL_CFG_THREAD_SIGNAL == ENABLED
typedef void vsf_thread_sighandler_t(vsf_thread_t *thread, int sig);
#   endif

#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
declare_class(vsf_thread_cb_t)
typedef void vsf_thread_entry_t(vsf_thread_cb_t *thread);
#else
typedef void vsf_thread_entry_t(vsf_thread_t *thread);
#endif

//! \name thread control block
//! @{
def_class(vsf_thread_cb_t,

    public_member(
        vsf_thread_entry_t      *entry;
#if VSF_KERNEL_CFG_THREAD_STACK_LARGE == ENABLED
        uint32_t                stack_size;
#else
        uint16_t                stack_size;
#endif
        uint64_t                *stack;                 //!< stack must be 8byte aligned
    )

    private_member(
#if VSF_KERNEL_THREAD_USE_HOST == ENABLED
        vsf_arch_irq_thread_t   host_thread;
        vsf_arch_irq_request_t  req, *rep;
        vsf_evt_t               evt;
#   if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
        bool                    is_inited;
#   endif
#else
        jmp_buf                 *pos;
        jmp_buf                 *ret;
#endif
    )
)
end_def_class(vsf_thread_cb_t)
//! @}

#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED

def_class( vsf_thread_t,
    public_member(
#if VSF_KERNEL_CFG_EDA_SUPPORT_TIMER == ENABLED
        implement(vsf_teda_t)
#else
        implement(vsf_eda_t)
#endif
#if VSF_KERNEL_CFG_THREAD_SIGNAL == ENABLED
        vsf_thread_sighandler_t *sighandler;
#endif
    )
#if VSF_KERNEL_CFG_THREAD_SIGNAL == ENABLED && VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
    private_member(
        // set sig_pending if current thread is in subcall,
        //  sighandler will be called if sig_pending is set and subcall returns.
        VSF_KERNEL_CFG_THREAD_SIGNAL_MASK_T sig_pending;
    )
#endif
)
end_def_class(vsf_thread_t)

typedef struct {
    vsf_thread_entry_t          *entry;
    void                        *stack;
    uint_fast32_t               stack_size;
    //vsf_prio_t                  priority;         //!< TODO do we need this??
} vsf_thread_prepare_cfg_t;


#else

//! \name thread
//! @{
def_class(vsf_thread_t,
    public_member(
#if VSF_KERNEL_CFG_EDA_SUPPORT_TIMER == ENABLED
        implement(vsf_teda_t)
#else
        implement(vsf_eda_t)
#endif
#if VSF_KERNEL_CFG_THREAD_SIGNAL == ENABLED
        vsf_thread_sighandler_t *sighandler;
#endif
        implement(vsf_thread_cb_t)
    )

#if VSF_KERNEL_CFG_THREAD_SIGNAL == ENABLED && VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
    private_member(
        // set sig_pending if current thread is in subcall,
        //  sighandler will be called if sig_pending is set and subcall returns.
        VSF_KERNEL_CFG_THREAD_SIGNAL_MASK_T sig_pending;
    )
#endif
)
end_def_class(vsf_thread_t)
//! @}
#endif
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vk_eda_call_thread_prepare")
extern vsf_err_t vk_eda_call_thread_prepare(    vsf_thread_cb_t *thread_cb,
                                                vsf_thread_prepare_cfg_t *cfg);

VSF_CAL_SECTION(".text.vsf.kernel.vk_eda_call_thread")
extern vsf_err_t vk_eda_call_thread(vsf_thread_cb_t *thread_cb);

#if VSF_KERNEL_CFG_EDA_SUPPORT_TASK == ENABLED && VSF_KERNEL_CFG_EDA_SUBCALL_HAS_RETURN_VALUE == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vk_thread_call_task")
extern
fsm_rt_t vk_thread_call_task(vsf_task_entry_t task_handler, uintptr_t param, size_t local_size);
#endif

VSF_CAL_SECTION(".text.vsf.kernel.vk_thread_call_eda")
extern
vsf_err_t vk_thread_call_eda(   uintptr_t eda_handler,
                                uintptr_t param,
                                size_t local_size,
                                size_t local_buff_size,
                                uintptr_t local_buff);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_call_thread")
extern vsf_err_t vk_thread_call_thread(     vsf_thread_cb_t *thread_cb,
                                            vsf_thread_prepare_cfg_t *cfg);

#endif


VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread")
#if VSF_KERNEL_CFG_EDA_SUPPORT_SUB_CALL == ENABLED
extern vsf_err_t vsf_thread_start(  vsf_thread_t *thread,
                                    vsf_thread_cb_t *thread_cb,
                                    vsf_prio_t priority);
#else
extern vsf_err_t vsf_thread_start(vsf_thread_t *this_ptr, vsf_prio_t priority);
#endif

#if VSF_KERNEL_CFG_THREAD_STACK_CHECK == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_stack_check")
extern void vsf_thread_stack_check(void);
#endif

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_exit")
extern VSF_CAL_NO_RETURN void vsf_thread_exit(void);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_get_cur")
extern vsf_thread_t *vsf_thread_get_cur(void);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_wait")
extern vsf_evt_t vsf_thread_wait(void);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_wait_for_evt")
extern void vsf_thread_wait_for_evt(vsf_evt_t evt);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_sendevt")
extern void vsf_thread_sendevt(vsf_thread_t *thread, vsf_evt_t evt);

#if VSF_KERNEL_CFG_SUPPORT_EVT_MESSAGE == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_wait_for_evt_msg")
extern uintptr_t vsf_thread_wait_for_evt_msg(vsf_evt_t evt);
#endif

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_wait_for_evt_msg")
extern uintptr_t vsf_thread_wait_for_msg(void);

#if VSF_KERNEL_CFG_EDA_SUPPORT_TIMER == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_delay")
extern void vsf_thread_delay(vsf_systimer_tick_t tick);
#endif

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_yield")
void vsf_thread_yield(void);

#if VSF_KERNEL_CFG_SUPPORT_DYNAMIC_PRIOTIRY == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_set_priority")
extern vsf_prio_t vsf_thread_set_priority(vsf_prio_t priority);
#endif

#if VSF_KERNEL_CFG_THREAD_SIGNAL == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_signal")
extern void vsf_thread_signal(vsf_thread_t *thread, int sig);
#endif

#if VSF_KERNEL_CFG_SUPPORT_SYNC == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.__vsf_thread_wait_for_sync")
vsf_sync_reason_t __vsf_thread_wait_for_sync(vsf_sync_t *sync, vsf_timeout_tick_t time_out);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_mutex")
extern vsf_err_t vsf_thread_mutex_leave(vsf_mutex_t *mtx);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_queue")
vsf_sync_reason_t vsf_thread_queue_send(vsf_eda_queue_t *queue, void *node, vsf_timeout_tick_t timeout);

VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_queue")
vsf_sync_reason_t vsf_thread_queue_recv(vsf_eda_queue_t *queue, void **node, vsf_timeout_tick_t timeout);

#   if VSF_KERNEL_CFG_SUPPORT_BITMAP_EVENT == ENABLED
VSF_CAL_SECTION(".text.vsf.kernel.vsf_thread_bmpevt_pend")
extern vsf_sync_reason_t vsf_thread_bmpevt_pend(
                    vsf_bmpevt_t *bmpevt,
                    vsf_bmpevt_pender_t *pender,
                    vsf_timeout_tick_t timeout);
#   endif
#endif

#ifdef __cplusplus
}
#endif

#endif
#endif
