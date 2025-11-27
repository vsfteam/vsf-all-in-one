/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows in halfduplex
  mode how to use polling transfer data. use the mode switch to realize spi and
  i2s communication.
  the pins connection as follow:
  - spi2/i2s2                spi3/i2s3 
    pb12(ws)        <--->     pa4 (ws)
    pb13(sck)       <--->     pc10(sck)
    pc3(sd)         <--->     pc12(sd)

  for more detailed information. please refer to the application note document AN0102.
