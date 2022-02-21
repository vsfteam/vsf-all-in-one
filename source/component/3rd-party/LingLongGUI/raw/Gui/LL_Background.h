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

#ifndef _XBACKGROUND_H_
#define _XBACKGROUND_H_

#include "LL_General.h"


typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isColor:1;
    llColor color;
    uint32_t imageAddr;
}llBackground;


llBackground *llBackgroundQuickCreate(uint16_t nameId,int16_t width,int16_t height,bool isColor,llColor color,uint32_t imageAddr);
llBackground *llBackgroundCreate(uint16_t nameId);
void nBackgroundSetColor(uint16_t nameId, llColor color);
void nBackgroundSetImage(uint16_t nameId, uint32_t imageAddr);
void nBackgroundDelete(uint16_t nameId);
void pBackgroundRefresh(llBackground *widget);
void nBackgroundRefresh(uint16_t nameId);
void pBackgroundMove(llBackground *widget,int16_t x,int16_t y);
void nBackgroundMove(uint16_t nameId,int16_t x,int16_t y);
void pBackgroundSpecificAreaRecover(llBackground *widget,llGeometry geometry);

#endif //_XBACKGROUND_H_
