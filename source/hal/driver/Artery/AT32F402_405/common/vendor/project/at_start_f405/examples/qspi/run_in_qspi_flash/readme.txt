/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, using on-board
  en25qh128a as qspi flash, the program will init qspi xip mode, and the program
  in qspi will toggle the three leds.

  executing this project£¬step should be follow.
  step : 
  1. get qspi flash algorithm file. open BSP/utilities/xxxx_qspi_algorithm_demo/keil_flm/mdk_v5
     project and compile with selecting correct project targets and qspi pins.
  2. copy BSP/utilities/xxxx_qspi_algorithm_demo/keil_flm/mdk_v5/*.FLM to Keil install path/ARM/Flash
  3. add the homologous algorithm file of qspi flash at the keil flash download interface.
      (1) click "Options for target..." button.
      (2) chose "Debug" item and then click "Settings" button.
      (3) chose "Flash Download" item and then click "Add" button.
      (4) select homologous algorithm file of qspi flash and then click "Ok".

  the pins connection as follow:
  - qspi sck ---> pb2
  - qspi cs  ---> pc11
  - qspi io0 ---> pc9
  - qspi io1 ---> pb7
  - qspi io2 ---> pc8
  - qspi io3 ---> pc5

  for more detailed information. please refer to the application note document AN0088.
