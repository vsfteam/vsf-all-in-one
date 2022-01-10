/*
 * Copyright 2021 Ou Jianbo 59935554@qq.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * additional license
 * If you use this software to write secondary development type products,
 * must be released under GPL compatible free software license or commercial license.
*/

#ifndef _LL_BUTTONEX_H_
#define _LL_BUTTONEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_Button.h"



#define BUTTON_EVENT_DOUBLE_CLICK    5 //检测按键双击信号
#define BUTTON_EVENT_REPEAT_TIMES    6 //获取连击次数
#define BUTTON_EVENT_HOLD_TIME_MS       7 //获取按键按住不放的时间
#define BUTTON_EVENT_LONG_SHOOT      9 //检测按键长按触发一次和连续触发的信号



//配置参数

#define BUTTON_LONG_MS          1000  //长按触发时间
#define BUTTON_LONG_SHOOT_MS    100   //长按连续触发间隔
#define BUTTON_CLICK_TIME_OUT_MS  500 //连击超时时间

void pButtonExInit(llButton *widget);
void nButtonExInit(uint16_t nameId);
void llButtonExLoop(void);
uint16_t nButtonExGetState(uint16_t nameId,uint8_t state);
void llButtonExClear(void);

#ifdef __cplusplus
}
#endif

#endif //_LL_BUTTONEX_H_
