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

#ifndef _LL_PORT_H_
#define _LL_PORT_H_

#include "stdint.h"
#include "stdbool.h"
#include "LL_Config.h"

bool llClickGetPoint(int16_t *x,int16_t *y);
    
void llSetPoint(int16_t x,int16_t y,llColor color);
llColor llGetPoint(int16_t x,int16_t y);
void llFillSingleColor(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color);
void llFillMultipleColors(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor *color);
void llDrawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color);


//Ë«»º³å
void llDoubleBufferStart(void);

void llDoubleBufferEnd(bool isRefreshNow);

void llDoubleBufferClear(void);

void llDoubleBufferRefresh(void);
    
#endif //_LL_PORT_H_






