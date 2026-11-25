/**
  **************************************************************************
  * @file     readme.txt 
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, systick used for 
  delay function. qspi command port operate erase/write/read with polling,
  if qspi test pass, led3 fresh, else led2 fresh.
  the qspi flash is en25qh128a. 
  - qspi io0   --->   pc9
  - qspi io1   --->   pb7
  - qspi io2   --->   pc8
  - qspi io3   --->   pc5
  - qspi sck   --->   pb2
  - qspi cs    --->   pc11

  for more detailed information. please refer to the application note document AN0088.