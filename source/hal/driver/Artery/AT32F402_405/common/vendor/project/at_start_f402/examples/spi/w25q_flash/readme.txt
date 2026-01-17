/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use spi access 
  the w25q flash chip.
  the pins connection as follow:
  - spi2                     w25qxx
    pb12(cs)       <--->     cs pin
    pb13(sck)      <--->     clk pin
    pc2(miso)      <--->     di pin
    pc3(mosi)      <--->     do pin

  for more detailed information. please refer to the application note document AN0102.

