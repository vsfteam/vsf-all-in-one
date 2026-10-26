/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, enable SRAM odd parity
  error lock. The SRAM odd parity error status will be set and trigger the TMR 
  brake function when the SRAM error occur, and trigger nmi exception.

  note: It is recommended to enable SRAM odd parity check by using programming
  tools during programming, rather than by modifying code in USD area. 