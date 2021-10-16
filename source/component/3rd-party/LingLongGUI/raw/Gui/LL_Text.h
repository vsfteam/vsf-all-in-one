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

#ifndef _LL_TEXT_H_
#define _LL_TEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"
#include "LL_Characters.h"

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isLineBreak:1;
    llChar textInfo;
    llGeometry sysGeometry;//实际显示的坐标参数,用于临时记录
    int16_t moveHorizontal;
    int16_t moveVertical;//本次偏移值,用于临时记录
    int16_t offsetHorizontal;
    int16_t offsetVertical;//上次偏移值,用于临时记录
    llPoint sysClickedPos;
}llText;

llText *llTextQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                            uint8_t *text,llFontLib *fontLib,llColor textColor,llColor bgColor,
                            llHorizontalAlign hAlign,llVerticalAlign vAlign,
                            int16_t moveHorizontal,int16_t moveVertical,
                            bool isAutoLineBreak,bool isTransparent,bool isHidden);
llText *llTextCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height, uint8_t *text, llFontLib *fontLib,bool isHidden);

void pTextSetTransparent(llText *widget,bool isTransparent);
void nTextSetTransparent(uint16_t nameId,bool isTransparent);

void pTextSetFont(llText *widget,llFontLib *fontLib);
void nTextSetFont(uint16_t nameId, llFontLib *fontLib);

void pTextSetTextColor(llText *widget,llColor color);
void nTextSetTextColor(uint16_t nameId,llColor color);

void pTextSetBackgroundColor(llText *widget,llColor color);
void nTextSetBackgroundColor(uint16_t nameId, llColor color);

void pTextSetText(llText *widget,uint8_t *text);
void nTextSetText(uint16_t nameId,uint8_t *text);

uint8_t* pTextGetText(llText *widget);
uint8_t* nTextGetText(uint16_t nameId);

void pTextRefresh(llText *widget);
void nTextRefresh(uint16_t nameId);

void pTextSetAlign(llText *widget,llHorizontalAlign hAlign,llVerticalAlign vAlign);
void nTextSetAlign(uint16_t nameId,llHorizontalAlign hAlign,llVerticalAlign vAlign);

void pTextSetEnabled(llText *widget, bool state);
void nTextSetEnabled(uint16_t nameId, bool state);

void pTextSetHidden(llText *widget,bool isHidden);
void nTextSetHidden(uint16_t nameId,bool isHidden);

#ifdef __cplusplus
}
#endif

#endif //_LL_TEXT_H_
