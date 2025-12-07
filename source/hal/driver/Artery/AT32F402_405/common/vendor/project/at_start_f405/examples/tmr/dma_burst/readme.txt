/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to update
  the tmr1 channel1 period and the duty cycle using the tmr1 dma burst feature.

  every update dma request, the dma will do 3 transfers of half words into timer
  registers beginning from pr register.
  on the dma update request, 0x0fff will be transferred into pr, 0x0000
  will be transferred into rpr, 0x0555 will be transferred into c1dt.

  tmr1 configuration: generate 1 pwm signal using the dma burst mode:
  the tmr1clk frequency is set to apb2_freq(hz),the prescaler is computed
  as following:
   - prescaler = (tim1clk / tmr1 counter clock) - 1
  tmr1 frequency = tmr1 counter clock/(pr + 1) = apb2_freq / 4096 = 41.01khz
  tmr1 channel1 duty cycle = (tim1_c1dt/ tmr1_pr)* 100 = 33.33%

  set-up
  connect the following pins to an oscilloscope to monitor the different
  waveforms:
  - tim1 ch1 ---> pa8

  for more detailed information. please refer to the application note document AN0085.
