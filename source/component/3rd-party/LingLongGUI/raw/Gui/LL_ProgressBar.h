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

#ifndef _LL_PROGRESSBAR_H_
#define _LL_PROGRESSBAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"
#include "LL_Characters.h"

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    llColor backgroundColor;
    llColor progressColor;
    uint8_t percent;
    bool isMicroRefresh;
    uint16_t percentWidth;
    llChar textInfo;
}llProgressBar;

llProgressBar *llProgressBarQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                        llFontLib *fontLib,llColor textColor,llColor bgColor,llColor progressColor,
                                        uint8_t percent,bool isHidden);
llProgressBar *llProgressBarCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height, llFontLib *fontLib,bool isHidden);

uint8_t pProgressBarGetPercent(llProgressBar* widget);
uint8_t nProgressBarGetPercent(uint16_t nameId);

void pProgressBarSetPercent(llProgressBar* widget,uint8_t percent);
void nProgressBarSetPercent(uint16_t nameId,uint8_t percent);
void pProgressBarSetEnabled(llProgressBar *widget, bool state);
void nProgressBarSetEnabled(uint16_t nameId, bool state);
void pProgressBarSetHidden(llProgressBar *widget,bool isHidden);
void nProgressBarSetHidden(uint16_t nameId,bool isHidden);
#ifdef __cplusplus
}
#endif










#endif //_LL_PROGRESSBAR_H_
