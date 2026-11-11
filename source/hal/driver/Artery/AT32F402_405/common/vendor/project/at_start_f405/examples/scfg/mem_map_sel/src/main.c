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

/* includes */
#include "at32f402_405_board.h"
#include "at32f402_405_clock.h"

/** @addtogroup AT32F405_periph_examples
  * @{
  */

/** @addtogroup 405_SCFG_mem_map_sel SCFG_mem_map_sel
  * @{
  */

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  uint8_t mem_map;
  system_clock_config();

  at32_board_init();

  /* enable the scfg clock */
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
  
  mem_map = scfg_mem_map_get();

  while(1)
  {
    switch (mem_map)
    {      
      case SCFG_MEM_MAP_BOOT_MEMORY:
        at32_led_toggle(LED3);
      break;
      
      case SCFG_MEM_MAP_INTERNAL_SRAM:
        at32_led_toggle(LED4);
      break;
      
      default:
        at32_led_toggle(LED2);
      break;
    }
    delay_ms(100);
  }
}


/**
  * @}
  */

/**
  * @}
  */
