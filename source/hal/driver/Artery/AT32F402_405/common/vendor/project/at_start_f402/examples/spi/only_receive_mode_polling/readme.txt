/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  only receive mode receive data by polling mode.
  the pins connection as follow:
  - spi2 slaver              spi3 master
    pb12(cs)       <--->     pa4(cs)
    pb13(sck)      <--->     pc10(sck)
    pc3(mosi)      <--->     pc12(mosi)

  for more detailed information. please refer to the application note document AN0102.

