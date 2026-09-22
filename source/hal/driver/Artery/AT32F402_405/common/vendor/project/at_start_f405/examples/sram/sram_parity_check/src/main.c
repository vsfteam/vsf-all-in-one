/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f402_405_board.h"
#include "at32f402_405_clock.h"

/** @addtogroup AT32F405_periph_examples
  * @{
  */

/** @addtogroup 405_SRAM_parity_check SRAM_parity_check
  * @{
  */

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */

int main(void)
{
  system_clock_config();
  at32_board_init();
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
  if((flash_ssb_status_get()&0x80) == (uint8_t)USD_RAM_PRT_CHK_DISABLE)
  {
    flash_unlock();
    flash_user_system_data_erase();
    flash_ssb_set(USD_WDT_ATO_DISABLE | USD_DEPSLP_NO_RST | USD_STDBY_NO_RST | USD_BOOT1_LOW | USD_DEPSLP_WDT_CONTINUE | USD_STDBY_WDT_CONTINUE | USD_RAM_PRT_CHK_ENABLE);
    nvic_system_reset();
  }
  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
