# CMakeLists head

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cec.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_comp.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cordic.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cryp.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cryp_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dac.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dac_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dcmi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dfsdm.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dfsdm_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dsi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dts.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fdcan.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fmac.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gfxmmu.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hash.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hash_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hrtim.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_irda.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_iwdg.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_jpeg.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_lptim.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdios.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nand.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nor.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_opamp.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ospi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_otfdec.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pssi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ramecc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rng.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rng_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smartcard.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smartcard_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smbus.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smbus_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spdifrx.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_swpmi.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_usart.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_usart_ex.c
    STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_wwdg.c

    vsf_bsp.c
)
