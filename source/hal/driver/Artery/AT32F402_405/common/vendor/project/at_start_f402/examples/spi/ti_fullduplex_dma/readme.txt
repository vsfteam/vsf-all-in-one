/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  fullduplex mode transfer data by dma mode in ti mode.
  the pins connection as follow:
  - spi2                     spi3 
    pb12(cs)       <--->     pa4(cs)
    pb13(sck)      <--->     pc10(sck)
    pc2(miso)      <--->     pc11 (miso)
    pc3(mosi)      <--->     pc12(mosi)

  for more detailed information. please refer to the application note document AN0102.
