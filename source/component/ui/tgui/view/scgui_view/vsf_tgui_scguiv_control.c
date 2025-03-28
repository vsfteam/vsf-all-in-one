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
    &&  VSF_TGUI_CFG_RENDERING_TEMPLATE_SEL == VSF_TGUI_V_TEMPLATE_SCGUI_VIEW

#define __VSF_TGUI_CONTROLS_CONTROL_CLASS_INHERIT
#define __VSF_TGUI_CLASS_INHERIT
#include "component/ui/tgui/vsf_tgui.h"

#include "./vsf_tgui_scguiv_control.h"

#include "./scgui.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


fsm_rt_t vsf_tgui_control_v_init(vsf_tgui_t* gui_ptr, vsf_tgui_control_t* control_ptr)
{
    return fsm_rt_cpl;
}

VSF_CAL_WEAK(vsf_tgui_v_get_background_color)
vsf_tgui_v_color_t vsf_tgui_v_get_background_color(vsf_tgui_control_t* control_ptr)
{
#if VSF_TGUI_CFG_V_SUPPORT_FLUXIBLE_BACKGROUND_COLOR == ENABLED
    vsf_tgui_v_color_t color = control_ptr->background_color;

    if (control_ptr->id == VSF_TGUI_COMPONENT_ID_BUTTON) {
        vsf_tgui_button_t* button_ptr = (vsf_tgui_button_t*)control_ptr;
        if (button_ptr->_.bIsChecked) {
            color = alphaBlend(color, VSF_TGUI_CFG_V_BUTTON_CLICKED_MIX_COLOR,
                VSF_TGUI_CFG_V_BUTTON_CLICKED_MIX_VALUE);
        }
    }

    return color;
#else
    return VSF_TGUI_COLOR_BLUE;
#endif
}

fsm_rt_t vsf_tgui_control_v_render(     vsf_tgui_t* gui_ptr,
                                        vsf_tgui_control_t* control_ptr,
                                        vsf_tgui_region_t* dirty_region_ptr,       //!< you can ignore the tDirtyRegion for simplicity
                                        vsf_tgui_control_refresh_mode_t mode)
{
//    VSF_TGUI_LOG(VSF_TRACE_INFO, "[SCgui View]%s(%p) control view rendering at ((%d, %d), (%d, %d))" VSF_TRACE_CFG_LINEEND,
//        control_ptr->node_name_ptr, control_ptr, control_ptr->iX, control_ptr->iY, control_ptr->iWidth, control_ptr->iHeight);

    if (control_ptr->Status.Values.__skip_render_frame) {
        return fsm_rt_cpl;
    }

    vsf_tgui_v_color_t bg_color = vsf_tgui_v_get_background_color(control_ptr);
    vsf_tgui_v_color_t border_color = control_ptr->border_width ? control_ptr->border_color : bg_color;

    vsf_tgui_region_t draw_abs_region = { 0 };
    vsf_tgui_control_calculate_absolute_location(control_ptr, &draw_abs_region.tLocation);

    __vsf_tgui_v_update_dirty_region(gui_ptr, control_ptr, dirty_region_ptr);

    if (control_ptr->border_radius > 0) {
        SC_pfb_RoundFrame(&gui_ptr->cur_tile,
            draw_abs_region.iX, draw_abs_region.iY,
            draw_abs_region.iX + control_ptr->iWidth - 1,
            draw_abs_region.iY + control_ptr->iHeight - 1,
            control_ptr->border_radius - 1, control_ptr->border_radius - control_ptr->border_width - 1,
            border_color, bg_color);
    } else {
        SC_pfb_RectFrame(&gui_ptr->cur_tile,
            draw_abs_region.iX, draw_abs_region.iY,
            draw_abs_region.iX + control_ptr->iWidth - 1,
            draw_abs_region.iY + control_ptr->iHeight - 1,
            control_ptr->border_width,
            border_color, bg_color);
    }

    const vsf_tgui_tile_t* ptTile = control_ptr->tBackground.ptTile;
    if (ptTile != NULL) {
#if VSF_TGUI_CFG_SV_SUPPORT_TILE_TRANSPARENCY == ENABLED
        uint8_t tile_trans_rate = control_ptr->tile_trans_rate;
#else
        uint8_t tile_trans_rate = 0xFF;
#endif
        vsf_tgui_control_v_draw_tile(gui_ptr, control_ptr, dirty_region_ptr, ptTile, control_ptr->tBackground.tAlign, tile_trans_rate, NULL,
            (vsf_tgui_v_color_t){ 0 }, bg_color);
    }
    return fsm_rt_cpl;
}

fsm_rt_t vsf_tgui_control_v_post_render(vsf_tgui_t *gui_ptr,
                                        vsf_tgui_control_t* control_ptr,
                                        vsf_tgui_region_t* dirty_region_ptr,
                                        vsf_tgui_control_refresh_mode_t mode)
{
    return fsm_rt_cpl;
}

fsm_rt_t vsf_tgui_control_v_depose(vsf_tgui_t* gui_ptr, vsf_tgui_control_t* control_ptr)
{
    return fsm_rt_cpl;
}

fsm_rt_t vsf_tgui_control_v_update(vsf_tgui_t* gui_ptr, vsf_tgui_control_t* control_ptr)
{
    return fsm_rt_cpl;
}

bool __vk_tgui_control_v_is_transparent_in_region(vsf_tgui_control_t *control_ptr, vsf_tgui_region_t* region_ptr)
{
    // TODO:
#if VSF_TGUI_CFG_V_SUPPORT_ROUND_BORDER == ENABLED
    if (NULL == region_ptr) {
        return control_ptr->border_radius > 0;
    }
    return      (   (region_ptr->iX > control_ptr->border_radius)
                &&  (region_ptr->iX + region_ptr->iWidth) < (control_ptr->iWidth - control_ptr->border_radius))
            ||  (   (region_ptr->iY > control_ptr->border_radius)
                &&  (region_ptr->iY + region_ptr->iHeight) < (control_ptr->iHeight - control_ptr->border_radius));
#else
    return false;
#endif
}

#endif


/* EOF */
