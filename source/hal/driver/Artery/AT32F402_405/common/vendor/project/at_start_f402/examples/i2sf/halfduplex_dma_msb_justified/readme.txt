/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to constitute
  a halfduplex i2s module by i2sf module.
  the pins distribution as follow:
  - halfduplex i2s
  - i2sf5 master        spi i2s slaver
  - pb6(ws)      <--->     pa4(ws)
  - pb7(ck)      <--->     pa5(ck) 
  - pb9(sd)      <--->     pa7(sd)
  - pc8(mck) 

  for more detailed information. please refer to the application note document AN0102.
