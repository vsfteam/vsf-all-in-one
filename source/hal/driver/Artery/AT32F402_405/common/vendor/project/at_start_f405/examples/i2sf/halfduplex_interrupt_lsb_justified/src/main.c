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

/** @addtogroup 405_I2SF_fullduplex_dma_philip I2SF_fullduplex_dma_philip
  * @{
  */

#define TXBUF_SIZE                       32
#define RXBUF_SIZE                       TXBUF_SIZE

uint16_t i2sf5_buffer_tx[TXBUF_SIZE];
uint16_t spi_i2s_buffer_rx[RXBUF_SIZE];
__IO uint32_t tx_index = 0, rx_index = 0;
volatile error_status transfer_status1 = ERROR, transfer_status2 = ERROR;

/**
  * @brief  buffer compare function.
  * @param  none
  * @retval the result of compare
  */
error_status buffer_compare(uint16_t* pbuffer1, uint16_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return ERROR;
    }

    pbuffer1++;
    pbuffer2++;
  }
  return SUCCESS;
}

/**
  * @brief  buffer_compare_24bits function.
  * @param  none
  * @retval the result of compare
  */
error_status buffer_compare_24bits(uint16_t* pbuffer1, uint16_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      if(*pbuffer1 != (*pbuffer2 & 0xFF00))
      {
        return ERROR;
      }
    }

    pbuffer1++;
    pbuffer2++;
  }
  return SUCCESS;
}

/**
  * @brief  transfer data fill.
  * @param  none
  * @retval none
  */
void tx_data_fill(void)
{
  uint32_t data_index = 0;
  for(data_index = 0; data_index < TXBUF_SIZE; data_index++)
  {
    i2sf5_buffer_tx[data_index] = data_index + 1;
  }
}

/**
  * @brief  spi i2s configuration.
  * @param  format: the data channel format to be choose
  * @param  freq: the audio sampling freq to be choose
  * @retval none
  */
static void spi_i2s_config(i2s_data_channel_format_type format, i2s_audio_sampling_freq_type freq)
{
  i2s_init_type i2s_init_struct;
  
  /* spi i2s initialization */
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(SPI1_IRQn, 0, 0);
  spi_i2s_reset(SPI1);
  i2s_default_para_init(&i2s_init_struct);
  
  /* i2s1 as slave reception */
  i2s_init_struct.audio_protocol = I2S_AUDIO_PROTOCOL_LSB;
  i2s_init_struct.audio_sampling_freq = freq;
  i2s_init_struct.data_channel_format = format;
  i2s_init_struct.clock_polarity = I2S_CLOCK_POLARITY_LOW;
  i2s_init_struct.mclk_output_enable = TRUE;
  i2s_init_struct.i2s_ckin_value = (uint32_t)8000000;
  i2s_init_struct.pcm_sample_clock_selection = I2S_PCM_SAMPLE_CLOCK_FALLING;
  i2s_init_struct.operation_mode = I2S_MODE_SLAVE_RX;
  i2s_init(SPI1, &i2s_init_struct);
  
  /* enable receive data buffer full interrupt */
  spi_i2s_interrupt_enable(SPI1, SPI_I2S_RDBF_INT, TRUE);
}

/**
  * @brief  spi i2s gpio configuration.
  * @param  none
  * @retval none
  */
static void spi_i2s_gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);

  /* spi_i2s ws pin */
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pins = GPIO_PINS_4;
  gpio_init(GPIOA, &gpio_initstructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_5);

  /* spi_i2s ck pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins = GPIO_PINS_5;
  gpio_init(GPIOA, &gpio_initstructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE5, GPIO_MUX_5);

  /* spi_i2s sd pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_initstructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE7, GPIO_MUX_5);
}

/**
  * @brief  i2sf configuration.
  * @param  none
  * @retval none
  */
static void i2sf_config(i2s_data_channel_format_type format, i2s_audio_sampling_freq_type freq)
{
  i2s_init_type i2s_init_struct;
  
  /* i2sf initialization */
  crm_periph_clock_enable(CRM_I2SF5_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(I2SF5_IRQn, 0, 0);
  spi_i2s_reset(I2SF5);
  i2s_default_para_init(&i2s_init_struct);
  
  /* i2sf5 clock source select */
  crm_i2sf5_clock_select(CRM_I2SF5_CLOCK_SOURCE_SCLK);

  /* i2sf5 as master transmission */
  i2s_init_struct.operation_mode = I2S_MODE_MASTER_TX;
  i2s_init_struct.audio_protocol = I2S_AUDIO_PROTOCOL_LSB;
  i2s_init_struct.audio_sampling_freq = freq;
  i2s_init_struct.data_channel_format = format;
  i2s_init_struct.clock_polarity = I2S_CLOCK_POLARITY_LOW;
  i2s_init_struct.mclk_output_enable = TRUE;
  
  /* when choose external clock,the external clock on ckin pin need to be 12.288MHz */
  i2s_init_struct.i2s_ckin_value = (uint32_t)12288000;
  i2s_init_struct.pcm_sample_clock_selection = I2S_PCM_SAMPLE_CLOCK_FALLING;
  i2s_init(I2SF5, &i2s_init_struct);
  
  /* enable transmit data buffer empty interrupt */
  spi_i2s_interrupt_enable(I2SF5, SPI_I2S_TDBE_INT, TRUE);
}

/**
  * @brief  i2sf gpio configuration.
  * @param  none
  * @retval none
  */
static void i2sf_gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);

  /* i2sf ws pin */
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOB, &gpio_initstructure);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE6, GPIO_MUX_6);

  /* i2sf ck pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOB, &gpio_initstructure);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE7, GPIO_MUX_6);

  /* i2sf mck pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins = GPIO_PINS_8;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE8, GPIO_MUX_6);

  /* i2sf sd pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins = GPIO_PINS_9;
  gpio_init(GPIOB, &gpio_initstructure);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE9, GPIO_MUX_6);
}

/**
  * @brief  spi1 interrupt function
  * @param  none
  * @retval none
  */
 void SPI1_IRQHandler(void)
{
  if(spi_i2s_interrupt_flag_get(SPI1, SPI_I2S_RDBF_FLAG) != RESET)
  {
    spi_i2s_buffer_rx[rx_index++] = spi_i2s_data_receive(SPI1);
  }
}

/**
  * @brief  i2sf5 interrupt function
  * @param  none
  * @retval none
  */
void I2SF5_IRQHandler(void)
{
  if(spi_i2s_interrupt_flag_get(I2SF5, SPI_I2S_TDBE_FLAG) != RESET)
  {
    spi_i2s_data_transmit(I2SF5, i2sf5_buffer_tx[tx_index++]);
    if(tx_index == TXBUF_SIZE)
    {
      spi_i2s_interrupt_enable(I2SF5, SPI_I2S_TDBE_INT, FALSE);
    }
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;
  system_clock_config();
  at32_board_init();
  at32_led_on(LED4);

  tx_data_fill();
  
  /* gpio config: spi_i2s gpio and i2sf gpio */
  spi_i2s_gpio_config();
  i2sf_gpio_config();
  
  /* spi_i2s cofig */
  spi_i2s_config(I2S_DATA_16BIT_CHANNEL_32BIT, I2S_AUDIO_FREQUENCY_48K);
  
  /* i2sf cofig */
  i2sf_config(I2S_DATA_16BIT_CHANNEL_32BIT, I2S_AUDIO_FREQUENCY_48K);
  
  i2s_enable(SPI1, TRUE);
  i2s_enable(I2SF5, TRUE);
  
  /* wait slave data receive end */
  while(rx_index < 32);
  
  /* wait master and slave idle when communication end */
  while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) != RESET);
  while(spi_i2s_flag_get(I2SF5, SPI_I2S_BF_FLAG) != RESET);
  
  /* test result:the data check */
  transfer_status1 = buffer_compare(spi_i2s_buffer_rx, i2sf5_buffer_tx, TXBUF_SIZE);
  
  /* receive buffer clear */
  tx_index = 0;
  rx_index = 0;
  for(index = 0; index < 32; index++)
  {
    spi_i2s_buffer_rx[index] = 0;
  }
  
  spi_i2s_config(I2S_DATA_24BIT_CHANNEL_32BIT, I2S_AUDIO_FREQUENCY_16K);
  i2sf_config(I2S_DATA_24BIT_CHANNEL_32BIT, I2S_AUDIO_FREQUENCY_16K);
  
  i2s_enable(SPI1, TRUE);
  i2s_enable(I2SF5, TRUE);
  
  /* wait slave data receive end */
  while(rx_index < 32);
  
  /* wait master and slave idle when communication end */
  while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) != RESET);
  while(spi_i2s_flag_get(I2SF5, SPI_I2S_BF_FLAG) != RESET);

  /* test result:the data check */
  transfer_status2 = buffer_compare_24bits(spi_i2s_buffer_rx, i2sf5_buffer_tx, TXBUF_SIZE);
  
  /* test result indicate:if success ,led2 lights */
  if((transfer_status1 == SUCCESS) && (transfer_status2 == SUCCESS))
  {
    at32_led_on(LED2);
  }
  else
  {
    at32_led_on(LED3);
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
