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
#include "../../vsf_tgui_cfg.h"

#if     VSF_USE_TINY_GUI == ENABLED \
    &&  VSF_TGUI_CFG_RENDERING_TEMPLATE_SEL == VSF_TGUI_V_TEMPLATE_EXAMPLE

#define __VSF_TGUI_CONTROLS_PANEL_CLASS_INHERIT
declare_class(vsf_tgui_t)
#include "./vsf_tgui_v_panel.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


fsm_rt_t vsf_tgui_panel_v_init(vsf_tgui_t *gui_ptr, vsf_tgui_panel_t* panel_ptr)
{
    if (fsm_rt_cpl == vsf_tgui_container_v_init(gui_ptr, &(panel_ptr->use_as__vsf_tgui_container_t))) {
#if VSF_TGUI_CFG_PANEL_HAS_LABEL == ENABLED
    #if VSF_TGUI_CFG_SUPPORT_CONTROL_LAYOUT_PADDING == ENABLED
        int16_t iWidth = panel_ptr->iWidth - panel_ptr->tContainerPadding.chLeft - panel_ptr->tContainerPadding.chRight;
    #else
        int16_t iWidth = panel_ptr->iWidth;
    #endif
        panel_ptr->tTitleLabel.iWidth = iWidth;
        panel_ptr->tTitleLabel.iHeight = 32;

        return vk_tgui_label_init(&(panel_ptr->tTitleLabel));
#else
        return vsf_tgui_control_v_init(gui_ptr, &panel_ptr->use_as__vsf_tgui_control_t);
#endif
    }
    return fsm_rt_on_going;
}

fsm_rt_t vsf_tgui_panel_v_render(   vsf_tgui_t *gui_ptr,
                                    vsf_tgui_panel_t* panel_ptr,
                                    vsf_tgui_region_t* dirty_region_ptr,       //!< you can ignore the tDirtyRegion for simplicity
                                    vsf_tgui_control_refresh_mode_t mode)
{

    return vsf_tgui_container_v_render(gui_ptr, &panel_ptr->use_as__vsf_tgui_container_t, dirty_region_ptr, mode);
}

fsm_rt_t vsf_tgui_panel_v_post_render(  vsf_tgui_t *gui_ptr,
                                        vsf_tgui_panel_t* panel_ptr,
                                        vsf_tgui_region_t* dirty_region_ptr,
                                        vsf_tgui_control_refresh_mode_t mode)
{
    return fsm_rt_cpl;
}

fsm_rt_t vsf_tgui_panel_v_depose(vsf_tgui_t *gui_ptr, vsf_tgui_panel_t* panel_ptr)
{
    return fsm_rt_cpl;
}

fsm_rt_t vsf_tgui_panel_v_update(vsf_tgui_t *gui_ptr, vsf_tgui_panel_t* panel_ptr)
{
    return fsm_rt_cpl;
}

#endif


/* EOF */
