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

#ifndef __HAL_DRIVER_GIGADEVICE_GD32H7XX_SDIO_H__
#define __HAL_DRIVER_GIGADEVICE_GD32H7XX_SDIO_H__

/*============================ INCLUDES ======================================*/

#include "hal/vsf_hal_cfg.h"

#if VSF_HAL_USE_SDIO == ENABLED

#include "../__device.h"

/*============================ MACROS ========================================*/

#define VSF_SDIO_CFG_REIMPLEMENT_TYPE_TRANSOP           ENABLED
#define VSF_SDIO_CFG_REIMPLEMENT_TYPE_IRQ_MASK          ENABLED
#define VSF_SDIO_CFG_REIMPLEMENT_TYPE_TRANSACT_STATUS   ENABLED

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef enum vsf_sdio_transop_t {
    SDIO_CMDOP_BYTE                     = 0,
    SDIO_CMDOP_STREAM                   = 0,
    SDIO_CMDOP_SINGLE_BLOCK             = (1ul << 8),   // MMC_CMDOP_RW
    SDIO_CMDOP_MULTI_BLOCK              = (1ul << 8) | (1ul << 10) | (1ul << 16),
                                                        // MMC_CMDOP_RW | SDMMC_MULTI_BLOCK_MODE | SDMMC_AUTOCMD12_ENABLE
    SDIO_CMDOP_WRITE                    = (1ul << 9),   // SDMMC_WRITE_MODE

    __SDIO_CMDOP_RESP                   = (1ul << 4),   // SDMMC_RESPONSE_ENABLE
    __SDIO_CMDOP_RESP_BUSY              = 0,
    __SDIO_CMDOP_RESP_SHORT             = (1ul << 5),   // SDMMC_CONFIG_R3
    __SDIO_CMDOP_RESP_SHORT_CRC         = (0ul << 5),   // SDMMC_CONFIG_Rx
    __SDIO_CMDOP_RESP_LONG_CRC          = (2ul << 5),   // SDMMC_CONFIG_R2
#define SDIO_RESP_R1                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT_CRC)
#define SDIO_RESP_R1B                   (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT_CRC | __SDIO_CMDOP_RESP_BUSY)
#define SDIO_RESP_R2                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_LONG_CRC)
#define SDIO_RESP_R3                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT)
#define SDIO_RESP_R4                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT)
#define SDIO_RESP_R5                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT_CRC)
#define SDIO_RESP_R6                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT_CRC)
#define SDIO_RESP_R7                    (__SDIO_CMDOP_RESP | __SDIO_CMDOP_RESP_SHORT_CRC)

    SDIO_CMDOP_CLKHOLD                  = (1ul << 6),
    SDIO_CMDOP_TRANS_STOP               = (1ul << 7),
} vsf_sdio_transop_t;

typedef enum vsf_sdio_irq_mask_t {
    SDIO_IRQ_MASK_HOST_RESP_DONE        = (0x1ul << 9), // SDMMC_RESP_DONE_FLAG
    SDIO_IRQ_MASK_HOST_DATA_DONE        = (0x1ul << 4), // SDMMC_DATA_DONE_FLAG
    SDIO_IRQ_MASK_HOST_ALL              = SDIO_IRQ_MASK_HOST_RESP_DONE
                                        | SDIO_IRQ_MASK_HOST_DATA_DONE,
} vsf_sdio_irq_mask_t;

typedef enum vsf_sdio_transact_status_t {
    SDIO_TRANSACT_STATUS_DONE           = 0,
    SDIO_TRANSACT_STATUS_ERR_RESP_NONE  = (0x1ul << 9),
    SDIO_TRANSACT_STATUS_ERR_RESP_CRC   = (0x1ul << 8),
    SDIO_TRANSACT_STATUS_ERR_DATA_CRC   = (0xFFul << 16),
    SDIO_TRANSACT_STATUS_DATA_BUSY      = (0x1ul << 2),
    SDIO_TRANSACT_STATUS_BUSY           = (0x1ul << 1),
    SDIO_TRANSACT_STATUS_ERR_MASK       = SDIO_TRANSACT_STATUS_ERR_RESP_NONE
                                        | SDIO_TRANSACT_STATUS_ERR_RESP_CRC
                                        | SDIO_TRANSACT_STATUS_ERR_DATA_CRC,
} vsf_sdio_transact_status_t;

/*============================ INCLUDES ======================================*/

#endif      // VSF_HAL_USE_SDIO
#endif      // __HAL_DRIVER_GIGADEVICE_GD32H7XX_SDIO_H__