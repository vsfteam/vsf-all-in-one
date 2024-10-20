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

#ifndef __VSF_BLOCK_STREAM_H__
#define __VSF_BLOCK_STREAM_H__

/*============================ INCLUDES ======================================*/

#include "service/vsf_service_cfg.h"

#if VSF_USE_SIMPLE_STREAM == ENABLED && VSF_USE_FIFO == ENABLED

#include "../../fifo/vsf_fifo.h"
#include "utilities/vsf_utilities.h"

#if     defined(__VSF_BLOCK_STREAM_CLASS_IMPLEMENT)
#   define __VSF_CLASS_IMPLEMENT__
#elif   defined(__VSF_BLOCK_STREAM_CLASS_INHERIT__)
#   define __VSF_CLASS_INHERIT__
#endif

#include "utilities/ooc_class.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/

#if VSF_USE_FIFO != ENABLED
#   error block stream need VSF_USE_FIFO
#endif

#define vsf_block_stream_size_t             uint32_t

#define __VSF_BLOCK_STREAM_INIT(__BLOCK_NUM, __BLOCK_SIZE)                      \
            .op                 = &vsf_block_stream_op,                         \
            .block_size         = (vsf_fifo_item_size_t)(((__BLOCK_SIZE) + 3) & ~3),\
            .block_num          = (vsf_fifo_index_t)(__BLOCK_NUM),
#define VSF_BLOCK_STREAM_INIT(__BLOCK_NUM, __BLOCK_SIZE)                        \
            __VSF_BLOCK_STREAM_INIT((__BLOCK_NUM), (__BLOCK_SIZE))

#define __vsf_block_stream_type(__name)     __name##_block_stream_t
#define vsf_block_stream_type(__name)       __vsf_block_stream_type(__name)

#define __declare_block_stream(__name)                                          \
            vsf_dcl_class(vsf_block_stream_type(__name))                        \
            dcl_vsf_fifo(__name)

#define __define_block_stream(__name, __block_num, __block_size)                \
            typedef struct __name##_block_stream_item_t {                       \
                implement(vsf_block_stream_item_t)                              \
                uint32_t __buffer[(__block_size + 3) >> 2];                     \
            } __name##_block_stream_item_t;                                     \
            def_vsf_fifo(__name, __name##_block_stream_item_t, (__block_num))   \
            vsf_class(vsf_block_stream_type(__name)) {                          \
                public_member(                                                  \
                    implement(__vsf_block_stream_t)                             \
                )                                                               \
                private_member(                                                 \
                    vsf_fifo(__name) fifo;                                      \
                )                                                               \
            };

#define declare_block_stream(__name)                                            \
            __declare_block_stream(__name)
#define dcl_block_stream(__name)                                                \
            declare_block_stream(__name)
#define define_block_stream(__name, __block_num, __block_size)                  \
            __define_block_stream(__name, (__block_num), (__block_size))
#define def_block_stream(__name, __block_num, __block_size)                     \
            define_block_stream(__name, (__block_num), (__block_size))

#define __describe_block_stream(__name, __block_num, __block_size)              \
            declare_block_stream(__name)                                        \
            define_block_stream(__name, (__block_num), (__block_size))          \
            vsf_block_stream_type(__name) __name = {                            \
                VSF_BLOCK_STREAM_INIT(__block_num, __block_size)                \
            };

#define describe_block_stream(__name, __block_num, __block_size)                \
            __describe_block_stream(__name, (__block_num), (__block_size))

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct vsf_block_stream_item_t {
    vsf_block_stream_size_t size;
    vsf_block_stream_size_t pos;
} vsf_block_stream_item_t;

vsf_class(__vsf_block_stream_base_t) {
    public_member(
        implement(vsf_stream_t)
        vsf_fifo_item_size_t block_size;
        vsf_fifo_index_t block_num;
    )
    private_member(
        vsf_fifo_item_size_t data_size;
    )
};

vsf_class(__vsf_block_stream_t) {
    public_member(
        implement(__vsf_block_stream_base_t)
        // implementation of virtual class vsf_fifo_t
        vsf_fifo_base_t __fifo[0];
    )
};

/*============================ GLOBAL VARIABLES ==============================*/

extern const vsf_stream_op_t vsf_block_stream_op;

/*============================ PROTOTYPES ====================================*/

#ifdef __cplusplus
}
#endif

#undef __VSF_BLOCK_STREAM_CLASS_IMPLEMENT
#undef __VSF_BLOCK_STREAM_CLASS_INHERIT__

#endif      // VSF_USE_SIMPLE_STREAM
#endif      // __VSF_BLOCK_STREAM_H__
