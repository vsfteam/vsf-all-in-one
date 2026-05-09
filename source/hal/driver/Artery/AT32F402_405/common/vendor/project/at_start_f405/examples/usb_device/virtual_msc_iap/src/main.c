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
#include "usb_conf.h"
#include "usb_core.h"
#include "usbd_int.h"
#include "msc_class.h"
#include "msc_desc.h"
#include "flash_fat16.h"



/** @addtogroup AT32F405_periph_examples
  * @{
  */

/** @addtogroup 405_USB_device_virtual_msc_iap USB_device_virtual_msc_iap
  * @{
  */

/* usb global struct define */
otg_core_type otg_core_struct;
void (*pftarget)(void);
void usb_clock48m_select(usb_clk48_s clk_s);
void usb_gpio_config(void);
void usb_low_power_wakeup_config(void);
void button_exint_init(button_type button);
void button_isr(void);
void jump_to_app(uint32_t address);
void usb_re_connect(void);


/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();

  flash_fat16_init();

  at32_board_init();

  if(flash_fat16_get_upgrade_flag() == IAP_SUCCESS &&
    (at32_button_press() == NO_BUTTON))
  {
    jump_to_app(flash_iap.flash_app_addr);
  }

  /* usb gpio config */
  usb_gpio_config();

#ifdef USB_LOW_POWER_WAKUP
  usb_low_power_wakeup_config();
#endif

  /* enable otg clock */
  crm_periph_clock_enable(OTG_CLOCK, TRUE);

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HEXT);

  /* enable otg irq */
  nvic_irq_enable(OTG_IRQ, 0, 0);

  /* init usb */
  usbd_init(&otg_core_struct,
            USB_SPEED_CORE_ID,
            USB_ID,
            &msc_class_handler,
            &msc_desc_handler);

  while(1)
  {
    flash_fat16_loop_status();
  }
}

/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }
  else
  {
    crm_pllu_output_set(TRUE);
    /* wait till pll is ready */
    while(crm_flag_get(CRM_PLLU_STABLE_FLAG) != SET)
    {
    }
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLLU);
  }
}

/**
  * @brief  this function config gpio.
  * @param  none
  * @retval none
  */
void usb_gpio_config(void)
{
  gpio_init_type gpio_init_struct;

  crm_periph_clock_enable(OTG_PIN_GPIO_CLOCK, TRUE);
  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

#ifdef USB_SOF_OUTPUT_ENABLE
  crm_periph_clock_enable(OTG_PIN_SOF_GPIO_CLOCK, TRUE);
  gpio_init_struct.gpio_pins = OTG_PIN_SOF;
  gpio_init(OTG_PIN_SOF_GPIO, &gpio_init_struct);
  gpio_pin_mux_config(OTG_PIN_SOF_GPIO, OTG_PIN_SOF_SOURCE, OTG_PIN_MUX);
#endif

  /* otgfs use vbus pin */
#ifndef USB_VBUS_IGNORE
  gpio_init_struct.gpio_pins = OTG_PIN_VBUS;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_pin_mux_config(OTG_PIN_GPIO, OTG_PIN_VBUS_SOURCE, OTG_PIN_MUX);
  gpio_init(OTG_PIN_GPIO, &gpio_init_struct);
#endif


}
#ifdef USB_LOW_POWER_WAKUP
/**
  * @brief  usb low power wakeup interrupt config
  * @param  none
  * @retval none
  */
void usb_low_power_wakeup_config(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
  exint_default_para_init(&exint_init_struct);

  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select = OTG_WKUP_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_irq_enable(OTG_WKUP_IRQ, 0, 0);
}

/**
  * @brief  this function handles otgfs wakup interrupt.
  * @param  none
  * @retval none
  */
void OTG_WKUP_HANDLER(void)
{
  exint_flag_clear(OTG_WKUP_EXINT_LINE);
}

#endif


/**
  * @brief  usb re-connect
  * @param  none
  * @retval none
  */
void usb_reset()
{
  delay_ms(1000);
  usbd_disconnect(&otg_core_struct.dev);
  delay_ms(1000);
  usbd_connect(&otg_core_struct.dev);
}

/**
  * @brief  loop status
  * @param  none
  * @retval none
  */
void flash_fat16_loop_status(void)
{
  static uint8_t status = UPGRADE_READY;
  if(status == flash_iap.msc_up_status)
  {
    return;
  }
  else
  {
    status = flash_iap.msc_up_status;
  }

  switch(flash_iap.msc_up_status)
  {
    case UPGRADE_READY:
      flash_fat16_set_name((const uint8_t *)"Ready", 5);
      break;
    case UPGRAGE_ONGOING:
      break;
    case UPGRADE_SUCCESS:
      flash_fat16_set_name((const uint8_t *)"Success", 7);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_FAILED:
      flash_fat16_set_name((const uint8_t *)"Failed", 6);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_LARGE:
      flash_fat16_set_name((const uint8_t *)"Large", 5);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_UNKNOWN:
      flash_fat16_set_name((const uint8_t *)"Unkown", 6);
      break;
    case UPGRADE_DONE:
      break;
    case UPGRADE_JUMP:
      flash_fat16_set_name((const uint8_t *)"Jump", 6);
      delay_ms(1000);
      jump_to_app(flash_iap.write_addr);
      break;
    default:
      break;
  }

}


/**
  * @brief  jump to app
  * @param  none
  * @retval none
  */
void jump_to_app(uint32_t address)
{
  uint32_t stkptr, jumpaddr;
  stkptr = *(uint32_t *)address;
  jumpaddr = *(uint32_t *)(address + sizeof(uint32_t));

  /* disable nvic irq and periph clock, clear pending */
  nvic_irq_disable(OTGFS1_IRQn);

  __NVIC_ClearPendingIRQ(OTGFS1_IRQn);

  crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, FALSE);

  crm_periph_reset(CRM_OTGFS1_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_OTGFS1_PERIPH_RESET, FALSE);

  __set_MSP(stkptr);
  pftarget = (void (*) (void))jumpaddr;
  pftarget();
}

/**
  * @brief  this function handles otgfs interrupt.
  * @param  none
  * @retval none
  */
void OTG_IRQ_HANDLER(void)
{
  usbd_irq_handler(&otg_core_struct);
}

/**
  * @brief  usb delay millisecond function.
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{
  /* user can define self delay function */
  delay_ms(ms);
}

/**
  * @brief  usb delay microsecond function.
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}

/**
  * @}
  */

/**
  * @}
  */
