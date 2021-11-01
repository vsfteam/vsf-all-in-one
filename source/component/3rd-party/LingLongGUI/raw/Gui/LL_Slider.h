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

#ifndef _LL_SLIDER_H_
#define _LL_SLIDER_H_

#include "LL_General.h"
#include "LL_Characters.h"



typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isHorizontal:1;//水平方向
    bool isAutoMove:1;
    bool isColor;
    llColor backgroundColor;
    llColor foregroundColor;
    llColor cursorColor;
    uint32_t bgImgAddr;
    uint32_t fgImgAddr;
    uint32_t curImgAddr;
    llGeometry bgImgGeometry;
    llGeometry fgImgGeometry;
    llGeometry curImgGeometry;
    uint16_t movePos;
    uint8_t cursorWidth;
    uint8_t percent;
}llSlider;

llSlider *llSliderQuickCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y ,int16_t width, int16_t height,
                              llColor bgColor,llColor fgColor,llColor cursorColor,
                              uint32_t bgImgAddr,uint32_t fgImgAddr,uint32_t cursorImgAddr,
                            bool isColor,bool isHorizontal,bool isHidden);
llSlider *llSliderCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y ,int16_t width, int16_t height,bool isHorizontal,bool isHidden);
uint8_t pSliderGetPercent(llSlider* widget);
uint8_t nSliderGetPercent(uint16_t nameId);

void pSliderSetPercent(llSlider* widget,uint8_t percent);
void nSliderSetPercent(uint16_t nameId,uint8_t percent);
void pSliderSetEnabled(llSlider *widget, bool state);
void nSliderSetEnabled(uint16_t nameId, bool state);
void pSliderSetHidden(llSlider *widget,bool isHidden);
void nSliderSetHidden(uint16_t nameId,bool isHidden);
void pSliderSetAutoMove(llSlider *widget,bool isAutoMove);
void nSliderSetAutoMove(uint16_t nameId,bool isAutoMove);
uint8_t pSliderGetClickPercent(llSlider* widget);
uint8_t nSliderGetClickPercent(uint16_t nameId);
#endif //_LL_SLIDER_H_
