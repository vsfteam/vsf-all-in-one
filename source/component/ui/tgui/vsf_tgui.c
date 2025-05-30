/****************************************************************************
*  Copyright 2020 by Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)    *
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
#include "./vsf_tgui_cfg.h"

#if VSF_USE_TINY_GUI == ENABLED

#define __VSF_TGUI_CONTROLS_CONTROL_CLASS_INHERIT
#define __VSF_TGUI_CLASS_IMPLEMENT
#include "./vsf_tgui.h"

/*============================ MACROS ========================================*/

#if VSF_KERNEL_CFG_EDA_SUPPORT_TASK != ENABLED
#   error please enable VSF_KERNEL_CFG_EDA_SUPPORT_TASK for tgui
#endif

#if VSF_KERNEL_CFG_EDA_SUPPORT_PT != ENABLED
#   error please enable VSF_KERNEL_CFG_EDA_SUPPORT_PT for tgui
#endif

#undef this
#define this                    (*this_ptr)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

static void __vk_tgui_depose_top_container( vsf_tgui_t* gui_ptr,
                                            vsf_tgui_root_container_t *top_ptr,
                                            vsf_tgui_evt_t *evt_ptr);

static void __vk_tgui_change_actived_control(
                                        vsf_tgui_t* gui_ptr,
                                        const vsf_tgui_control_t *control_ptr);

/*============================ IMPLEMENTATION ================================*/

implement_vsf_rng_buf(__vsf_tgui_evt_queue_t, vsf_tgui_evt_t, __vsf_interrupt_safe)


vsf_err_t vk_tgui_init(vsf_tgui_t* gui_ptr, const vsf_tgui_cfg_t* cfg_ptr)
{
    vsf_err_t tErr = VSF_ERR_NONE;
    class_internal(gui_ptr, this_ptr, vsf_tgui_t);

    VSF_TGUI_ASSERT(NULL != gui_ptr);
    VSF_TGUI_ASSERT(NULL != cfg_ptr);

    do {
        if ((NULL == cfg_ptr->evt_queue.ptr.obj_ptr)
            || (cfg_ptr->evt_queue.s32_size <= sizeof(vsf_tgui_evt_t))
#if     VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_LAYER_BY_LAYER
            || (NULL == cfg_ptr->bfs_queue.ptr.obj_ptr)
            || (cfg_ptr->bfs_queue.s32_size <= 0)
#endif
            ) {

            tErr = VSF_ERR_INVALID_PARAMETER;
            break;
        }

        vsf_tgui_vport_init(&gui_ptr->use_as__vsf_tgui_v_port_t);

        //! initialise the queue
        vsf_rng_buf_prepare(__vsf_tgui_evt_queue_t,
            &this.msg_queue,
            (vsf_tgui_evt_t*)(cfg_ptr->evt_queue.ptr.obj_ptr),
            cfg_ptr->evt_queue.s32_size);

        do {
            const vsf_msgt_cfg_t cfg = {
                c_tControlInterfaces,
                dimof(c_tControlInterfaces),
            };
            vsf_msgt_init(&(this.use_as__vsf_msgt_t), &cfg);
        } while (0);

#if     VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_LAYER_BY_LAYER
        vsf_msgt_forward_propagate_msg_bfs_init(
            &(this.use_as__vsf_msgt_t),
            (uint16_t*)(cfg_ptr->bfs_queue.ptr.obj_ptr),
            cfg_ptr->bfs_queue.s32_size, false);
#elif   VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_Z_ORDER
        vsf_msgt_forward_propagate_msg_pre_order_traversal_init(
            &(this.use_as__vsf_msgt_t),
            false);
#endif

        vsf_msgt_forward_propagate_msg_dfs_init(&(this.use_as__vsf_msgt_t));

        //! initialise input
        do {
            this.input.finger_idx_pre = -1; /* -1 means no valid finger index */

        } while(0);

        //! initialise consumer pt task
        do {
            this.consumer.param.queue_ptr = &this.msg_queue;
            this.consumer.param.msg_tree_ptr = &this.use_as__vsf_msgt_t;

            if (NULL != cfg_ptr->root_node_ptr) {
                vk_tgui_set_root_container(gui_ptr, (vsf_tgui_root_container_t *)cfg_ptr->root_node_ptr, !cfg_ptr->is_ignore_first_refresh);
            }

            //! start pt task
            init_vsf_pt(__vsf_tgui_evt_shooter_t, &(this.consumer), cfg_ptr->priority);
        } while (0);

    } while (0);
    return tErr;
}


vsf_err_t vk_tgui_set_root_container(vsf_tgui_t* gui_ptr,
                                    vsf_tgui_root_container_t *root_node_ptr,
                                    bool req_first_refresh)
{
    VSF_TGUI_ASSERT(NULL != gui_ptr);
    VSF_TGUI_ASSERT(NULL != root_node_ptr);

    if (root_node_ptr == gui_ptr->consumer.param.root_node_ptr) {
        return VSF_ERR_ALREADY_EXISTS;
    }

    if (vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
            .msg = VSF_TGUI_EVT_ON_SET_TOP_CONTAINER,
            .target_ptr = (vsf_tgui_control_t*)root_node_ptr,
    })) {
        if (req_first_refresh) {
            if (vk_tgui_refresh_ex(gui_ptr, (const vsf_tgui_control_t *)root_node_ptr, NULL)) {
                return VSF_ERR_NONE;
            }
        } else {
            return VSF_ERR_NONE;
        }
    }

    return VSF_ERR_NOT_ENOUGH_RESOURCES;
}

vsf_err_t vk_tgui_close_root_container(vsf_tgui_t* gui_ptr)
{
    VSF_TGUI_ASSERT(NULL != gui_ptr);

    if (NULL == gui_ptr->consumer.param.root_node_ptr) {
        return VSF_ERR_NONE;
    }

    if (vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
            .msg = VSF_TGUI_EVT_ON_DEPOSE,
            .target_ptr = (vsf_tgui_control_t*)gui_ptr->consumer.param.root_node_ptr,
    })) {
        return VSF_ERR_NONE;
    }

    return VSF_ERR_NOT_ENOUGH_RESOURCES;
}



/*! \brief tgui msg queue producer */

bool vk_tgui_send_message(vsf_tgui_t* gui_ptr, vsf_tgui_evt_t event)
{
    bool is_to_notify;
    class_internal(gui_ptr, this_ptr, vsf_tgui_t);
    if (NULL == gui_ptr) {
        return false;
    }

    if (!vsf_rng_buf_send_one(__vsf_tgui_evt_queue_t,
        this.consumer.param.queue_ptr,
        event)) {
        return false;
    }

    __vsf_interrupt_safe(
        is_to_notify = this.consumer.param.Attribute.is_queue_drain;
        if (is_to_notify) {
            this.consumer.param.Attribute.is_queue_drain = false;
        }
    )

    if (is_to_notify) {
        //! wake pt task up
        vsf_eda_post_evt(&(this.consumer.use_as__vsf_pt_t.use_as__vsf_eda_t),
            VSF_TGUI_MSG_AVAILABLE);
    }

    return true;
}

bool vk_tgui_update(vsf_tgui_t* gui_ptr,
                    const vsf_tgui_control_t* target_ptr)
{

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
    vsf_tgui_evt_t event = { { {VSF_TGUI_EVT_UPDATE} } };
    event.use_as__vsf_tgui_msg_t.target_ptr = (vsf_tgui_control_t*)target_ptr;
    return vk_tgui_send_message(gui_ptr, event);

#else
    return vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
        .msg = VSF_TGUI_EVT_UPDATE,
            .target_ptr = (vsf_tgui_control_t*)target_ptr,
    });
#endif
}

bool vk_tgui_send_timer_event(  vsf_tgui_t* gui_ptr,
                                const vsf_tgui_control_t* target_ptr)
{

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
    vsf_tgui_evt_t event = { { {VSF_TGUI_EVT_ON_TIME} } };
    event.use_as__vsf_tgui_msg_t.target_ptr = (vsf_tgui_control_t*)target_ptr;
    return vk_tgui_send_message(gui_ptr, event);

#else
    return vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
        .msg = VSF_TGUI_EVT_ON_TIME,
            .target_ptr = (vsf_tgui_control_t*)target_ptr,
    });
#endif
}

bool vk_tgui_update_tree(vsf_tgui_t* gui_ptr,
                        const vsf_tgui_control_t* target_ptr)
{

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
    vsf_tgui_evt_t event = { { {VSF_TGUI_EVT_UPDATE_TREE} } };
    event.use_as__vsf_tgui_msg_t.target_ptr = (vsf_tgui_control_t*)target_ptr;
    return vk_tgui_send_message(gui_ptr, event);

#else
    return vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
        .msg = VSF_TGUI_EVT_UPDATE_TREE,
            .target_ptr = (vsf_tgui_control_t*)target_ptr,
    });
#endif
}

#if VSF_TGUI_CFG_REFRESH_SCHEME != VSF_TGUI_REFRESH_SCHEME_NONE

bool vk_tgui_refresh_ex(vsf_tgui_t* gui_ptr,
                        const vsf_tgui_control_t* target_ptr,
                        const vsf_tgui_region_t* region_ptr)
{
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
    vsf_tgui_evt_t event = { { {VSF_TGUI_EVT_REFRESH} } };
    event.use_as__vsf_tgui_msg_t.target_ptr = (vsf_tgui_control_t*)target_ptr;
    event.RefreshEvt.region_ptr = region_ptr;
    return vk_tgui_send_message(gui_ptr, event);

#else
    return vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
        .RefreshEvt = {
            .msg = VSF_TGUI_EVT_REFRESH,
            .target_ptr = (vsf_tgui_control_t*)target_ptr,
            .region_ptr = (vsf_tgui_region_t*)region_ptr,
        },
    });
#endif
}

bool vk_tgui_refresh_dirty(vsf_tgui_t* gui_ptr,
                            vsf_tgui_control_t* target_ptr)
{
#if VSF_TGUI_CFG_SUPPORT_DIRTY_REGION == ENABLED
#   if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
    vsf_tgui_evt_t event = { { {VSF_TGUI_EVT_REFRESH} } };
    event.use_as__vsf_tgui_msg_t.target_ptr = (vsf_tgui_control_t*)target_ptr;
    event.RefreshEvt.refresh_dirty = true;
    return vk_tgui_send_message(gui_ptr, event);

#   else
    return vk_tgui_send_message(gui_ptr,
        (vsf_tgui_evt_t) {
        .RefreshEvt = {
            .msg = VSF_TGUI_EVT_REFRESH,
            .target_ptr = (vsf_tgui_control_t*)target_ptr,
            .refresh_dirty = true,
        },
    });
#   endif
#else
    return vk_tgui_refresh_dirty(gui_ptr, target_ptr, NULL);
#endif
}

bool vk_tgui_refresh(vsf_tgui_t* gui_ptr)
{
    return vk_tgui_refresh_ex(gui_ptr, NULL, NULL);
}
#endif



static fsm_rt_t __vsf_tgui_send_msg(vsf_tgui_t* gui_ptr,
                                    const vsf_tgui_control_t* control_ptr,
                                    vsf_tgui_evt_t* event_ptr)
{
    fsm_rt_t fsm_rt;
    vsf_msgt_t* msg_tree_ptr = &(gui_ptr->use_as__vsf_msgt_t);
    do {
        fsm_rt = vsf_msgt_backward_propagate_msg(
            msg_tree_ptr,
            (const vsf_msgt_node_t*)control_ptr,
            &event_ptr->use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t);
        if (fsm_rt_cpl == fsm_rt) {
            break;
        } else if (fsm_rt < 0) {
            //! msg is not handled
            break;
        } else if (fsm_rt >= fsm_rt_user) {
            break;
        }
    } while (1);

#if VSF_TGUI_CFG_REFRESH_SCHEME != VSF_TGUI_REFRESH_SCHEME_NONE
    if (fsm_rt >= 0) {
        //! update reference to the control which handles the message
        control_ptr = (const vsf_tgui_control_t* )
        vsf_msgt_backward_propagate_msg_get_last_node(msg_tree_ptr);

        if ((fsm_rt_t)VSF_TGUI_MSG_RT_REFRESH == fsm_rt) {
        #if VSF_TGUI_CFG_SHOW_REFRESH_EVT_LOG == ENABLED
            VSF_TGUI_LOG(VSF_TRACE_WARNING, " \tRequest Refresh\r\n");
        #endif
            vk_tgui_refresh_ex(gui_ptr, control_ptr, NULL);
        } else if ((fsm_rt_t)VSF_TGUI_MSG_RT_REFRESH_PARENT == fsm_rt) {
        #if VSF_TGUI_CFG_SHOW_REFRESH_EVT_LOG == ENABLED
            VSF_TGUI_LOG(VSF_TRACE_WARNING, " \tRequest Refresh Parent\r\n");
        #endif
            if (NULL == control_ptr->use_as__vsf_msgt_node_t.parent_ptr) {
                vk_tgui_refresh_ex(gui_ptr, control_ptr, NULL);
            } else {
                vk_tgui_refresh_ex(
                    gui_ptr,
                    (const vsf_tgui_control_t *)control_ptr->
                        use_as__vsf_msgt_node_t.parent_ptr,
                    NULL);
            }
        }
    }
#endif

    return fsm_rt;
}


static void __vk_tgui_send_dfs_msg( vsf_msgt_t* msg_tree_ptr,
                                    const vsf_msgt_node_t* node_ptr,
                                    vsf_tgui_evt_t* event_ptr)
{
    do {
        fsm_rt_t fsm_rt = vsf_msgt_forward_propagate_msg_dfs(
            msg_tree_ptr,
            node_ptr,
            &(event_ptr->use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t));
        if (fsm_rt_cpl == fsm_rt) {
            break;
        } else if (fsm_rt < 0) {
            //! msg is not handled
            break;
        }
    } while (1);
}

static void __vk_tgui_send_bfs_msg( vsf_msgt_t* msg_tree_ptr,
                                    const vsf_msgt_node_t* node_ptr,
                                    vsf_tgui_evt_t* event_ptr,
                                    uint_fast8_t u8_status_mask)
{
    do {
        fsm_rt_t fsm_rt = vsf_msgt_forward_propagate_msg_bfs(
            msg_tree_ptr,
            node_ptr,
            &(event_ptr->use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t),
            u8_status_mask);
        if (fsm_rt_cpl == fsm_rt) {
            break;
        } else if (fsm_rt < 0) {
            //! msg is not handled
            break;
        }
    } while (1);
}

static void __vk_tgui_send_pot_msg( vsf_msgt_t* msg_tree_ptr,
                                    const vsf_msgt_node_t* node_ptr,
                                    vsf_tgui_evt_t* event_ptr,
                                    uint_fast8_t u8_status_mask)
{
    do {
        fsm_rt_t fsm_rt = vsf_msgt_forward_propagate_msg_pre_order_traversal(
            msg_tree_ptr,
            node_ptr,
            &(event_ptr->use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t),
            u8_status_mask);
        if (fsm_rt_cpl == fsm_rt) {
            break;
        } else if (fsm_rt < 0) {
            //! msg is not handled
            break;
        }
    } while (1);
}

static void __vk_tgui_set_focus(vsf_tgui_t* gui_ptr,
                                const vsf_tgui_control_t* previous_ptr,
                                const vsf_tgui_control_t* current_ptr,
                                vsf_evt_t msg)
{
    vsf_tgui_evt_t temp_event = {0};// = { { {VSF_TGUI_EVT_LOST_ACTIVE}} };
    temp_event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg = msg + 1;

    //! send deactivated message first to previous node
    if (NULL != previous_ptr) {
        __vsf_tgui_send_msg(gui_ptr, previous_ptr, &temp_event);
    }

    if (NULL != current_ptr) {
        //! send activated message to current node
        temp_event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg = msg;

        __vsf_tgui_send_msg(gui_ptr, current_ptr, &temp_event);
    }
}

static void __vk_tgui_change_focus_control( vsf_tgui_t* gui_ptr,
                                            const vsf_tgui_control_t *control_ptr,
                                            __vk_tgui_focus_t *focus_ptr,
                                            vsf_evt_t msg)
{
    //class_internal(&(gui_ptr->consumer.param), this_ptr, vsf_pt(__vsf_tgui_evt_shooter_t));

    if (focus_ptr->current_ptr != control_ptr) {
        focus_ptr->previous_ptr = focus_ptr->current_ptr;
        focus_ptr->current_ptr = control_ptr;

        __vk_tgui_set_focus(gui_ptr,
            focus_ptr->previous_ptr,
            focus_ptr->current_ptr,
            msg);
    }
}


const vsf_tgui_control_t *vsf_tgui_actived_control_get(vsf_tgui_t *gui_ptr)
{
    VSF_TGUI_ASSERT(NULL != gui_ptr);
    return gui_ptr->consumer.param.Activated.current_ptr;
}

#if VSF_TGUI_CFG_SUPPORT_MOUSE_LIKE_EVENTS == ENABLED
const vsf_tgui_control_t *vsf_tgui_pointed_control_get(vsf_tgui_t *gui_ptr)
{
    VSF_TGUI_ASSERT(NULL != gui_ptr);
    return gui_ptr->consumer.param.pointer_above.current_ptr;
}
#endif

bool vsf_tgui_set_pointer_location(  vsf_tgui_t *gui_ptr,
                                        int_fast8_t idx,
                                        vsf_tgui_location_t *location_ptr)
{
    VSF_TGUI_ASSERT(NULL != gui_ptr);
    VSF_TGUI_ASSERT(idx < dimof(gui_ptr->input.current));

    if (idx < 0) {
        idx = 0;
    }

    if (NULL == location_ptr) {
        location_ptr = gui_ptr->input.current + idx;
    }

    return __vk_tgui_send_touch_evt(gui_ptr,
                                    idx,
                                    VSF_TGUI_EVT_POINTER_MOVE,
                                    *location_ptr);
}

const vsf_tgui_location_t *vsf_tgui_get_pointer_location(vsf_tgui_t *gui_ptr, int_fast8_t idx)
{
    VSF_TGUI_ASSERT(NULL != gui_ptr);

    if (idx < 0) {
        return (const vsf_tgui_location_t *)gui_ptr->input.current;
    } else if (idx < dimof(gui_ptr->input.current)) {
        return (const vsf_tgui_location_t *)gui_ptr->input.current + idx;
    }

    return NULL;
}


void vsf_tgui_low_level_refresh_ready(vsf_tgui_t *gui_ptr)
{
#if VSF_TGUI_CFG_REFRESH_SCHEME != VSF_TGUI_REFRESH_SCHEME_NONE
    VSF_TGUI_ASSERT(NULL != gui_ptr);

    vsf_eda_post_evt(   &(gui_ptr->consumer.use_as__vsf_pt_t.use_as__vsf_eda_t),
                        VSF_TGUI_MSG_LOW_LEVEL_READY_TO_REFRESH);
#endif
}

static bool __vk_tgui_decide_refresh_region(vsf_pt(__vsf_tgui_evt_shooter_t) *this_ptr,
                                            vsf_tgui_control_t *control_ptr)
{
    bool result = true;
    this.region_ptr = NULL;

    if (!vsf_tgui_control_get_visible_region(control_ptr, &this.temp_region)) {
        return false;
    }

    vsf_tgui_region_t *refresh_region_ptr = this.event.RefreshEvt.region_ptr;
#if VSF_TGUI_CFG_SUPPORT_TRANSPARENT_CONTROL == ENABLED
    this.Attribute.is_dirty_region_include_transparent_area =
        __vk_tgui_control_v_is_transparent_in_region(control_ptr, refresh_region_ptr);
#endif

    if (NULL != refresh_region_ptr) {
        // this.event.RefreshEvt.region_ptr should point to relative region
        vsf_tgui_region_t region = *refresh_region_ptr;
        __vk_tgui_calculate_control_location_from_parent(control_ptr, NULL, &region.tLocation);
        result = vsf_tgui_region_intersect(&this.temp_region, &this.temp_region, &region);
    }

    this.region_ptr = &this.temp_region;
    return result;
}

bool __vk_tgui_update_focus(vsf_tgui_t* gui_ptr, bool exit_on_empty)
{
    //! check new target of activation
    gui_ptr->consumer.param.node_ptr = vsf_msgt_shoot_node(
                        gui_ptr->consumer.param.msg_tree_ptr,
                        (const vsf_msgt_node_t*)gui_ptr->consumer.param.root_node_ptr,
                        (uintptr_t) & (gui_ptr->input.current[0]));
    if (exit_on_empty && (NULL == gui_ptr->consumer.param.node_ptr)) {
        return false;
    }

#if VSF_TGUI_CFG_SUPPORT_MOUSE_LIKE_EVENTS == ENABLED
    __vk_tgui_change_focus_control(gui_ptr,
                        (const vsf_tgui_control_t *)gui_ptr->consumer.param.node_ptr,
                        &gui_ptr->consumer.param.pointer_above,
                        VSF_TGUI_EVT_POINTER_ENTER);
#endif
    return true;
}

/*! \brief tgui msg queue consumer */
implement_vsf_pt(__vsf_tgui_evt_shooter_t)
{
    vsf_tgui_t* gui_ptr = vsf_container_of(&vsf_this, vsf_tgui_t, consumer.param);
    bool result = false;
    vsf_evt_t   msg;
    vsf_pt_begin();

    vsf_this.root_node_ptr = NULL;
    while (1) {

loop_start:
        vsf_this.node_ptr = NULL;
        result = false;

        do {
            __vsf_interrupt_safe(
                result = vsf_rng_buf_get_one(__vsf_tgui_evt_queue_t, vsf_this.queue_ptr, &vsf_this.event);
                if (!result) {
                    vsf_this.Attribute.is_queue_drain = true;
                }
            )
            if (result) {
                break;
            }

            //! wait for new event arrival
            vsf_pt_wait_for_evt(VSF_TGUI_MSG_AVAILABLE);
        } while (1);

        msg = vsf_this.event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg;

        /*! \note Use user specific target_ptr, otherwise use RootNode */
        if (NULL != vsf_this.event.use_as__vsf_tgui_msg_t.target_ptr) {
            /*! use user specified target */
            vsf_this.node_ptr = (const vsf_msgt_node_t*)vsf_this.event.use_as__vsf_tgui_msg_t.target_ptr;
        } else if (NULL == vsf_this.root_node_ptr) {
            continue;
        } else {
            vsf_this.node_ptr = (const vsf_msgt_node_t*)vsf_this.root_node_ptr;
        }

        switch (msg & VSF_TGUI_MSG_MSK) {
            case VSF_TGUI_MSG_CONTROL_EVT& VSF_TGUI_MSG_MSK:

                switch (msg & VSF_TGUI_EVT_MSK) {

                    case VSF_TGUI_EVT_ON_SET_TOP_CONTAINER & VSF_TGUI_EVT_MSK:
                #if VSF_TGUI_CFG_SUPPORT_DESTRUCTOR_SCHEME == ENABLED
                        if (NULL != vsf_this.root_node_ptr) {
                             __vk_tgui_depose_top_container(gui_ptr,
                                                            (vsf_tgui_root_container_t *)vsf_this.root_node_ptr,
                                                            NULL);
                        }
                #endif
                        vsf_this.root_node_ptr = NULL;
                        vsf_this.Activated.current_ptr = NULL;
                        vsf_this.Activated.previous_ptr = NULL;
                        vsf_this.pointer_above.current_ptr = NULL;
                        vsf_this.pointer_above.previous_ptr = NULL;
                        do {
                            if (NULL == vsf_this.event.use_as__vsf_tgui_msg_t.target_ptr) {
                                break;
                            } else if (!vsf_this.node_ptr->Attribute._.is_top) {
                                break;
                            }

                            //! set new top container
                            vsf_this.Activated.current_ptr = (const vsf_tgui_control_t *)vsf_this.node_ptr;
                            vsf_this.pointer_above.current_ptr =  (const vsf_tgui_control_t *)vsf_this.node_ptr;

                            vsf_this.root_node_ptr = (const vsf_tgui_root_container_t *)vsf_this.node_ptr;
                            ((vsf_tgui_root_container_t *)vsf_this.root_node_ptr)->gui_ptr = gui_ptr;

                    #if VSF_TGUI_CFG_SUPPORT_CONSTRUCTOR_SCHEME == ENABLED
                            //! send on load message
                            vsf_this.event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg = VSF_TGUI_EVT_ON_LOAD;

                    #   if VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_BREADTH_FIRST_TRAVERSAL
                            vsf_msgt_forward_propagate_msg_bfs_setting(vsf_this.msg_tree_ptr, false);
                            /*! constructor message */
                            __vk_tgui_send_bfs_msg( vsf_this.msg_tree_ptr,
                                                    vsf_this.node_ptr,
                                                    &vsf_this.event,
                                                    0);
                    #   elif VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_PRE_ORDER_TRAVERSAL
                            vsf_msgt_forward_propagate_msg_pre_order_traversal_setting(vsf_this.msg_tree_ptr, false);
                            /*! constructor message */
                            __vk_tgui_send_pot_msg( vsf_this.msg_tree_ptr,
                                                    vsf_this.node_ptr,
                                                    &vsf_this.event,
                                                    0);
                    #   endif
                    #endif

                            vsf_this.event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg = VSF_TGUI_EVT_UPDATE_TREE;
                            __vk_tgui_send_dfs_msg(vsf_this.msg_tree_ptr, vsf_this.node_ptr, &vsf_this.event);

                        } while(0);
                        goto loop_start;

                #if VSF_TGUI_CFG_SUPPORT_CONSTRUCTOR_SCHEME == ENABLED
                    case VSF_TGUI_EVT_ON_LOAD & VSF_TGUI_EVT_MSK:
                #   if VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_BREADTH_FIRST_TRAVERSAL
                        vsf_msgt_forward_propagate_msg_bfs_setting(vsf_this.msg_tree_ptr, false);
                        /*! constructor message */
                        __vk_tgui_send_bfs_msg( vsf_this.msg_tree_ptr,
                                                vsf_this.node_ptr,
                                                &vsf_this.event,
                                                0);
                #   elif VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_PRE_ORDER_TRAVERSAL
                        vsf_msgt_forward_propagate_msg_pre_order_traversal_setting(vsf_this.msg_tree_ptr, false);
                        /*! constructor message */
                        __vk_tgui_send_pot_msg( vsf_this.msg_tree_ptr,
                                                vsf_this.node_ptr,
                                                &vsf_this.event,
                                                0);
                #   endif
                        goto loop_start;
                #endif


                #if VSF_TGUI_CFG_SUPPORT_DESTRUCTOR_SCHEME == ENABLED
                    case VSF_TGUI_EVT_ON_DEPOSE & VSF_TGUI_EVT_MSK: {
                        __vk_tgui_depose_top_container(gui_ptr, (vsf_tgui_root_container_t *)vsf_this.node_ptr, &vsf_this.event);

                        goto loop_start;
                    }
                #endif
                    case VSF_TGUI_EVT_ON_TIME & VSF_TGUI_EVT_MSK:
                        __vsf_tgui_send_msg(gui_ptr, (const vsf_tgui_control_t*)vsf_this.node_ptr, &vsf_this.event);
                        break;

                    case VSF_TGUI_EVT_UPDATE & VSF_TGUI_EVT_MSK:

                        if (!vsf_tgui_control_status_get((vsf_tgui_control_t*)vsf_this.node_ptr).Values.__does_contain_builtin_structure) {
                            //! send message
                            __vsf_tgui_send_msg(gui_ptr, (const vsf_tgui_control_t*)vsf_this.node_ptr, &vsf_this.event);
                            //goto loop_start;
                            break;
                        }
                        //vsf_this.event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg = VSF_TGUI_EVT_UPDATE_TREE;
                        // fall through

                    case VSF_TGUI_EVT_UPDATE_TREE & VSF_TGUI_EVT_MSK:
                        //! send message
                        __vk_tgui_send_dfs_msg(vsf_this.msg_tree_ptr, vsf_this.node_ptr, &vsf_this.event);
                        goto loop_start;

        #if VSF_TGUI_CFG_REFRESH_SCHEME != VSF_TGUI_REFRESH_SCHEME_NONE
                    case VSF_TGUI_EVT_REFRESH & VSF_TGUI_EVT_MSK:
                        if (NULL != vsf_this.event.use_as__vsf_tgui_msg_t.target_ptr) {
                            /*! use user specified target */
                            /*! do not refresh it if the control doesn't belong to current top container*/
                            if (vsf_this.root_node_ptr != vk_tgui_control_get_top((const vsf_tgui_control_t*)vsf_this.node_ptr)) {
                                goto loop_start;
                            }
                        }

                        vsf_tgui_region_t relative_dirty_region;
                        vsf_tgui_control_t *control_ptr = (vsf_tgui_control_t *)vsf_this.node_ptr;
        #   if VSF_TGUI_CFG_SUPPORT_DIRTY_REGION == ENABLED
                        if (vsf_this.event.RefreshEvt.refresh_dirty) {
                            vsf_this.event.RefreshEvt.refresh_dirty = false;
                            relative_dirty_region = control_ptr->tDirtyRegion;
                            vsf_this.event.RefreshEvt.region_ptr = &relative_dirty_region;
                            control_ptr->tDirtyRegion.iWidth = 0;
                        }
        #   endif
                        // vsf_this.event.RefreshEvt.region_ptr is based on vsf_this.node_ptr.
                        //  So if vsf_this.node_ptr is changed, vsf_this.event.RefreshEvt.region_ptr MUST be updated
                        vsf_tgui_region_t *relative_dirty_region_ptr = vsf_this.event.RefreshEvt.region_ptr;

                        if (!__vk_tgui_decide_refresh_region(&vsf_this, control_ptr)) {
                            goto loop_start;
                        }

        #   if   VSF_TGUI_CFG_SUPPORT_TRANSPARENT_CONTROL == ENABLED
                        bool bIsRequestRefreshParent = vsf_this.Attribute.is_dirty_region_include_transparent_area;

                        if (bIsRequestRefreshParent) {
                            if (NULL != control_ptr->parent_ptr) {
                                if (relative_dirty_region_ptr != NULL) {
                                    relative_dirty_region_ptr->iX += control_ptr->iX;
                                    relative_dirty_region_ptr->iY += control_ptr->iY;
                                }
                                vsf_this.node_ptr = (const vsf_msgt_node_t*)control_ptr->parent_ptr;
                                control_ptr = (vsf_tgui_control_t *)vsf_this.node_ptr;
                            }
                        }

                        //! if the target control is transparent, refresh its parent
                        do {
                            if (    (control_ptr->Status.Values.bIsTransparent)
                                &&  (NULL != control_ptr->parent_ptr)) {
                                if (relative_dirty_region_ptr != NULL) {
                                    relative_dirty_region_ptr->iX += control_ptr->iX;
                                    relative_dirty_region_ptr->iY += control_ptr->iY;
                                }
                                vsf_this.node_ptr = (const vsf_msgt_node_t*)control_ptr->parent_ptr;
                                control_ptr = (vsf_tgui_control_t *)vsf_this.node_ptr;
                                bIsRequestRefreshParent = true;
                                continue;
                            }
                            break;
                        } while (true);

                        if (bIsRequestRefreshParent) {
                            vsf_tgui_container_t *container_ptr = (vsf_tgui_container_t *)control_ptr;
                            if (container_ptr->ContainerAttribute.is_forced_to_refresh_whole_background) {
                                if (!__vk_tgui_decide_refresh_region(&vsf_this, control_ptr)) {
                                    goto loop_start;
                                }
                            }
                        }
        #   endif

                        goto refresh_loop;
        #endif

                    case VSF_TGUI_EVT_GET_ACTIVE& VSF_TGUI_EVT_MSK:
                        __vk_tgui_change_focus_control( gui_ptr,
                                                        (const vsf_tgui_control_t *)vsf_this.node_ptr,
                                                        &vsf_this.Activated,
                                                        VSF_TGUI_EVT_GET_ACTIVE);
                        goto loop_start;

                    default:
                    case VSF_TGUI_EVT_LOST_ACTIVE& VSF_TGUI_EVT_MSK:
                        //case VSF_TGUI_EVT_ON_LOAD & VSF_TGUI_EVT_MSK:
                            //! message we don't need to handle
                        goto loop_start;
                }
                break;

            case VSF_TGUI_MSG_POINTER_EVT & VSF_TGUI_MSG_MSK:

                /*! \note specified target will not change current activated control*/
                if (NULL != vsf_this.event.use_as__vsf_tgui_msg_t.target_ptr) {
                    break;
                }

                //! send message to currently activated control
                vsf_this.node_ptr = &(vsf_this.Activated.current_ptr->use_as__vsf_msgt_node_t);
                VSF_TGUI_ASSERT( NULL != vsf_this.node_ptr );


                //! pointer up event could only be sent to active node
                if (VSF_TGUI_EVT_POINTER_DOWN == msg) {

                    if (!__vk_tgui_update_focus(gui_ptr, true)) {
                        goto loop_start;
                    }

                    /*! \IMPORTANT only pointer/finger 0 can active controls.
                     *             tGUI is mainly designed to support touch screen
                     *             rather than a device with a mouse. So when you
                     *             use PC to simulate a touchscreen, a right click
                     *             won't change the activated controls. It is a *KNOWN
                     *             ISSUE* and we will *NOT* fix vsf_this, because designing
                     *             an application with normal mouse support is out of
                     *             original scope.
                     */
                    if (0 == vsf_this.event.PointerEvt.idx) {
                        __vk_tgui_change_focus_control( gui_ptr,
                                        (const vsf_tgui_control_t *)vsf_this.node_ptr,
                                        &vsf_this.Activated,
                                        VSF_TGUI_EVT_GET_ACTIVE);
                    }
                }
            #if VSF_TGUI_CFG_SUPPORT_MOUSE_LIKE_EVENTS == ENABLED
                else if (VSF_TGUI_EVT_POINTER_MOVE == msg) {
                    if (vsf_this.event.PointerEvt.idx < dimof(gui_ptr->input.current)) {
                        gui_ptr->input.current[vsf_this.event.PointerEvt.idx] = vsf_this.event.PointerEvt.use_as__vsf_tgui_location_t;
                    }
                    __vk_tgui_update_focus(gui_ptr, false);
                #if VSF_TGUI_CFG_SUPPORT_MOUSE_MOVE_HANDLING == ENABLED
                    if (NULL == vsf_this.node_ptr) {
                        //! missed all node (control)
                        goto loop_start;
                    }
                #else
                    goto loop_start;
                #endif
                }
            #endif
                break;

            case VSF_TGUI_MSG_KEY_EVT & VSF_TGUI_MSG_MSK:
            case VSF_TGUI_MSG_GESTURE_EVT& VSF_TGUI_MSG_MSK:
            case VSF_TGUI_MSG_CONTROL_SPECIFIC_EVT & VSF_TGUI_MSG_MSK:
                if (NULL == vsf_this.event.use_as__vsf_tgui_msg_t.target_ptr) {
                    //! send message to currently activated control
                    vsf_this.node_ptr = &(vsf_this.Activated.current_ptr->use_as__vsf_msgt_node_t);
                    VSF_TGUI_ASSERT( NULL != vsf_this.node_ptr );
                }
                break;

            default:
                //! message we don't need to handle
                goto loop_start;
        }

        //! default message handling
        __vsf_tgui_send_msg(gui_ptr, (const vsf_tgui_control_t*)vsf_this.node_ptr, &vsf_this.event);

        continue;

    #if VSF_TGUI_CFG_REFRESH_SCHEME != VSF_TGUI_REFRESH_SCHEME_NONE
refresh_loop :
        do {
            vsf_tgui_region_t *region_ptr = NULL;

            do {
                region_ptr = vsf_tgui_v_refresh_loop_begin(gui_ptr, vsf_this.region_ptr);
                if (NULL == region_ptr) {
                    /*! wait until it is ready to refresh */
                    vsf_pt_wait_for_evt(VSF_TGUI_MSG_LOW_LEVEL_READY_TO_REFRESH);
                    region_ptr = NULL;
                } else if ((intptr_t)region_ptr == -1) {
                    /* if return -1, end refresh immediately */
                    goto refresh_loop_end;
                }
            } while(NULL == region_ptr);
            vsf_this.region_ptr = region_ptr;

            /* vsf_this statement is very important, do not remove */
            vsf_this.event.RefreshEvt.region_ptr = (vsf_tgui_region_t*)vsf_this.region_ptr;

            //! set first refresh node bit
            do {
                vsf_tgui_control_t* node_ptr = (vsf_tgui_control_t *)vsf_this.node_ptr;
                node_ptr->Status.Values.__is_the_first_node_for_refresh = true;
            } while (0);

        #if VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_BREADTH_FIRST_TRAVERSAL
            vsf_msgt_forward_propagate_msg_bfs_setting(vsf_this.msg_tree_ptr, true);
            __vk_tgui_send_bfs_msg( vsf_this.msg_tree_ptr,
                                    vsf_this.node_ptr,
                                    &vsf_this.event,
                                    (   VSF_TGUI_CTRL_STATUS_INITIALISED
                                    |   VSF_TGUI_CTRL_STATUS_VISIBLE
                                    ));
        #elif VSF_TGUI_CFG_REFRESH_SCHEME == VSF_TGUI_REFRESH_SCHEME_PRE_ORDER_TRAVERSAL
            vsf_msgt_forward_propagate_msg_pre_order_traversal_setting(vsf_this.msg_tree_ptr, true);
            __vk_tgui_send_pot_msg( vsf_this.msg_tree_ptr,
                                    vsf_this.node_ptr,
                                    &vsf_this.event,
                                    (   VSF_TGUI_CTRL_STATUS_INITIALISED
                                    |   VSF_TGUI_CTRL_STATUS_VISIBLE
                                    ));
        #endif
        } while (vsf_tgui_v_refresh_loop_end(gui_ptr));
refresh_loop_end:
        continue;
    #endif
    }



    vsf_pt_end();
}


static void __vk_tgui_depose_top_container( vsf_tgui_t* gui_ptr,
                                            vsf_tgui_root_container_t *top_ptr,
                                            vsf_tgui_evt_t *evt_ptr)
{
    vsf_tgui_evt_t event;
    class_internal(&(gui_ptr->consumer.param), this_ptr, vsf_pt(__vsf_tgui_evt_shooter_t));

    if (!top_ptr->use_as__vsf_tgui_container_t.
            use_as__vsf_msgt_container_t.
                use_as__vsf_msgt_node_t.Attribute._.is_top) {
        //! we only deal with top node
        return ;
    }

    if (NULL == evt_ptr) {
        evt_ptr = &event;
        event.use_as__vsf_tgui_msg_t.use_as__vsf_msgt_msg_t.msg = VSF_TGUI_EVT_ON_DEPOSE;
        event.use_as__vsf_tgui_msg_t.target_ptr = (vsf_tgui_control_t *)top_ptr;
    }

    //! remove reference to tGUI obj
    do {
        top_ptr->gui_ptr = NULL;
        if (top_ptr == this.root_node_ptr) {
            this.root_node_ptr = NULL;
        }
    } while(0);

    //! send message
    __vk_tgui_send_dfs_msg(this.msg_tree_ptr, (const vsf_msgt_node_t *)top_ptr, evt_ptr);


}



#endif


/* EOF */
