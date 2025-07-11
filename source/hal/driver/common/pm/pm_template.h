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

#include "hal/driver/common/template/vsf_template_pm.h"

/*============================ MACROFIED FUNCTIONS ===========================*/

#define VSF_HAL_TEMPLATE_DEC_NAME                     _pm
#define VSF_HAL_TEMPLATE_DEC_UPCASE_NAME              _PM

#if !defined(VSF_PM_CFG_DEC_PREFIX) && !defined(VSF_PM_CFG_DEC_DEVICE_PREFIX)
#   error "Please define VSF_PM_CFG_DEC_PREFIX or VSF_PM_CFG_DEC_DEVICE_PREFIX when using vsf_template_pm.h"
#endif

#if !defined(VSF_HAL_TEMPLATE_DEC_UPCASE_PREFIX) && !defined(VSF_PM_CFG_DEC_DEVICE_UPCASE_PREFIX)
#   error "Please define VSF_HAL_TEMPLATE_DEC_UPCASE_PREFIX or VSF_PM_CFG_DEC_DEVICE_UPCASE_PREFIX when using vsf_template_pm.h"
#endif

// PM always is single instance
#define VSF_HAL_TEMPLATE_DEC_COUNT                1
#define VSF_HAL_TEMPLATE_DEC_MASK                 1

#include "hal/driver/common/template/vsf_template_instance_declaration.h"

#undef VSF_PM_CFG_DEC_PREFIX
#undef VSF_PM_CFG_DEC_UPCASE_PREFIX
#undef VSF_PM_CFG_DEC_DEVICE_PREFIX
#undef VSF_PM_CFG_DEC_DEVICE_UPCASE_PREFIX

