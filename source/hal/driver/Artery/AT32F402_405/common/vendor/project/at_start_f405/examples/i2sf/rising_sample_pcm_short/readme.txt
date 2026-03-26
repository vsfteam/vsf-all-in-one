/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use rising sample
  function when pcm standard on i2sf module.
  the pins distribution as follow:
  - fullduplex i2s
  - pb6(ws)
  - pb7(ck)
  - pb9(sd)
  - pb8(sdext)
  - pc8(mck) 
  
  if you have only one at-start board for test, you can connect the pins as follow:
  - pb6(ws)  <--->  NC
  - pb7(ck)  <--->  NC
  - pc8(mck) <--->  NC
  - pb9(sd)  <--->  pb8(ext_sd)
  and then choose "I2SF_MASTER_BOARD" to check communication result.
	
  for more detailed information. please refer to the application note document AN0102.
