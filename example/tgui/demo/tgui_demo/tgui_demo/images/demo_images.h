/*****************************************************************************
 *   Copyright(C)2009-2024 by VSF Team                                       *
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

#ifndef __VSF_TINY_GUI_IMAGE_DATA_H__
#define __VSF_TINY_GUI_IMAGE_DATA_H__

/*============================ INCLUDES ======================================*/
#include "vsf.h"

#if VSF_USE_UI == ENABLED && VSF_USE_TINY_GUI == ENABLED
/*============================ MACROS ========================================*/

#define TILES_TEXT_LIST \
    "ic_autorenew_black_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_build_black_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_done_black_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_find_in_page_white_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_help_outline_black_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_https_white_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_power_settings_new_18pt\n" /*width: 18, height: 18, RGBA*/ \
    "ic_settings_applications\n" /*width: 24, height: 24, RGBA*/ \
    "ic_settings_bluetooth_white_18dp\n" /*width: 18, height: 18, RGBA*/ \
    "ic_settings_phone\n" /*width: 24, height: 24, RGBA*/ \
    "corner12\n" /*width: 24, height: 24, L*/ \
    "corner16\n" /*width: 32, height: 32, L*/ \
    "None"

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
extern const vsf_tgui_tile_buf_root_t ic_autorenew_black_18dp_RGBA; //ic_autorenew_black_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_build_black_18dp_RGBA; //ic_build_black_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_done_black_18dp_RGBA; //ic_done_black_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_find_in_page_white_18dp_RGBA; //ic_find_in_page_white_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_help_outline_black_18dp_RGBA; //ic_help_outline_black_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_https_white_18dp_RGBA; //ic_https_white_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_power_settings_new_18pt_RGBA; //ic_power_settings_new_18pt:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_settings_applications_RGBA; //ic_settings_applications:RGBA, width: 24, height: 24
extern const vsf_tgui_tile_buf_root_t ic_settings_bluetooth_white_18dp_RGBA; //ic_settings_bluetooth_white_18dp:RGBA, width: 18, height: 18
extern const vsf_tgui_tile_buf_root_t ic_settings_phone_RGBA; //ic_settings_phone:RGBA, width: 24, height: 24
extern const vsf_tgui_tile_buf_root_t corner12_L; //corner12:L, width: 24, height: 24
extern const vsf_tgui_tile_buf_root_t corner16_L; //corner16:L, width: 32, height: 32

extern const vsf_tgui_tile_t *tiles_list[12];

/*============================ PROTOTYPES ====================================*/


#endif

#endif
/* EOF */

