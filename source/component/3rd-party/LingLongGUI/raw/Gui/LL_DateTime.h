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

#ifndef _LL_DATE_TIME_H_
#define _LL_DATE_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"
#include "LL_Characters.h"

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    llChar textInfo;
    uint8_t *dateTimeFormat;
}llDateTime;

llDateTime *llDateTimeQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                          uint8_t *dateTimeFormat,
                          llFontLib *fontLib,llColor textColor,llColor bgColor,
                          llHorizontalAlign hAlign,llVerticalAlign vAlign,bool isTransparent,bool isHidden);

llDateTime *llDateTimeCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t *dateTimeFormat,llFontLib *fontLib,bool isHidden);

//llText *llTextCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height, uint8_t *text, llFontLib *fontLib,bool isHidden);

//void pDateTimeSetTransparent(llDateTime *widget,bool isTransparent);
//void nDateTimeSetTransparent(uint16_t nameId,bool isTransparent);

void pDateTimeSetFont(llDateTime *widget,llFontLib *fontLib);
void nDateTimeSetFont(uint16_t nameId, llFontLib *fontLib);

void pDateTimeSetBackgroundColor(llDateTime *widget,llColor color);
void nDateTimeSetBackgroundColor(uint16_t nameId, llColor color);

void pDateTimeRefresh(llDateTime *widget);
void nDateTimeRefresh(uint16_t nameId);

void pDateTimeSetAlign(llDateTime *widget,llHorizontalAlign hAlign,llVerticalAlign vAlign);
void nDateTimeSetAlign(uint16_t nameId,llHorizontalAlign hAlign,llVerticalAlign vAlign);

void pDateTimeSetEnabled(llDateTime *widget, bool state);
void nDateTimeSetEnabled(uint16_t nameId, bool state);

void pDateTimeSetHidden(llDateTime *widget,bool isHidden);
void nDateTimeSetHidden(uint16_t nameId,bool isHidden);


#ifdef __cplusplus
}
#endif

#endif //_LL_DATE_TIME_H_
