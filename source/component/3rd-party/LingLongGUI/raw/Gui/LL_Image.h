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

#ifndef _LL_IMAGE_H_
#define _LL_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "LL_General.h"
#include "LL_Handler.h"



typedef struct
{
    LLGENERAL_ATTRIBUTES;
    uint32_t imageAddr;
}llImage;

#define llImageQuickCreate(nameId,parentNameId,x,y,imageAddr,isHidden)    llImageCreate(nameId,parentNameId,x,y,imageAddr,isHidden)
llImage *llImageCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x, uint16_t y, uint32_t imageAddr,bool isHidden);

bool pImageLoad(llImage *widget, uint32_t imageAddr);
bool nImageLoad(uint16_t nameId, uint32_t imageAddr);

void pImageRefresh(llImage *widget);
void nImageRefresh(uint16_t nameId);
void pImageSetEnabled(llImage *widget, bool state);
void nImageSetEnabled(uint16_t nameId, bool state);
void pImageSetHidden(llImage *widget,bool isHidden);
void nImageSetHidden(uint16_t nameId,bool isHidden);
void pImageMove(llImage *widget,int16_t x, int16_t y);
void nImageMove(uint16_t nameId,int16_t x, int16_t y);
#ifdef __cplusplus
}
#endif

#endif //_LL_IMAGE_H_
