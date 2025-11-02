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

/** @addtogroup 405_CRM_pll_parameter_calculate CRM_pll_parameter_calculate
  * @{
  */

error_status calculate_status = ERROR;

/**
  * @brief  clkout configuration.
  * @param  none
  * @retval none
  */
void clkout_config(void)
{
  gpio_init_type gpio_init_struct;

  /* enable gpio port clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* clkout gpio init */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* config clkout division */
  crm_clkout_div_set(CRM_CLKOUT_DIV1_4, CRM_CLKOUT_DIV2_1);

  /* config clkout clock */
  crm_clock_out_set(CRM_CLKOUT_SCLK);
}

/**
  * @brief  system clock config 216mhz
  * @note   the system clock is configured as follow:
  *         system clock (sclk)   = (hext * pll_ns)/(pll_ms * pll_fp)
  *         system clock source   = pll (hext)
  *         - hext                = HEXT_VALUE
  *         - sclk                = 216000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 216000000
  *         - apb2div             = 1
  *         - apb2clk             = 216000000
  *         - apb1div             = 2
  *         - apb1clk             = 108000000
  *         pll_ns, pll_ms and pll_fr need calculate
  * @param  none
  * @retval none
  */
void system_clock_config_216mhz(void)
{
  uint16_t pll_ns, pll_ms, pll_fp;

  /* reset crm */
  crm_reset();

  /* config flash psr register */
  flash_psr_set(FLASH_WAIT_CYCLE_6);

  /* set power ldo output voltage to 1.3v */
  pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);

  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

   /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR)
  {
  }

  /* calculate pll parameter according to the function */
  calculate_status = crm_pll_parameter_calculate(CRM_PLL_SOURCE_HEXT, 216000000, &pll_ms, &pll_ns, &pll_fp);

  /* config pll clock resource */
  crm_pll_config(CRM_PLL_SOURCE_HEXT, pll_ns, pll_ms, (crm_pll_fp_type)pll_fp);

  /* config pllu div */
  crm_pllu_div_set(CRM_PLL_FU_18);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk, the maximum frequency of APB2 clock is 216 MHz */
  crm_apb2_div_set(CRM_APB2_DIV_1);

  /* config apb1clk, the maximum frequency of APB1 clock is 120 MHz */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }

  /* disable auto step mode */
  crm_auto_step_mode_enable(FALSE);

  /* update system_core_clock global variable */
  system_core_clock_update();

#ifdef AT32F405xx
  /*
    AT32405 OTGHS PHY not initialized, resulting in high power consumption
    Solutions:
    1. If OTGHS is not used, call the "reduce_power_consumption" function to reduce power consumption.
       PLL or HEXT should be enabled when calling this function.
       Example: reduce_power_consumption();

    2. If OTGHS is required, initialize OTGHS to reduce power consumption, without the need to call this function.

       for more detailed information. please refer to the faq document FAQ0148.
  */
#endif

#ifdef AT32F402xx
  /* reduce power comsumption */
  reduce_power_consumption();
#endif
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config_216mhz();

  at32_board_init();

  clkout_config();

  while(1)
  {
    if(calculate_status == ERROR)
    {
      at32_led_toggle(LED2);
    }
    else
    {
      at32_led_toggle(LED4);
    }
    delay_ms(500);
  }
}

/**
  * @}
  */

/**
  * @}
  */
