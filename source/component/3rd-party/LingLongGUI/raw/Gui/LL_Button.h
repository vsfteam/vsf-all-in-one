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


#ifndef _LL_BUTTON_H_
#define _LL_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"
#include "LL_Characters.h"

#define BUTTON_DIS_TYPE_TRANSPARENT  0
#define BUTTON_DIS_TYPE_COLOR        1
#define BUTTON_DIS_TYPE_IMG          2
#define BUTTON_DIS_TYPE_STYLE        3

#define BUTTON_BUZZER_NONE        0
#define BUTTON_BUZZER_PRESSED        1
#define BUTTON_BUZZER_RELEASED        2
#define BUTTON_BUZZER_ALL        3

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isCheckable:1;
    bool isChecked:1;
    bool isPressed:1;
    bool isSelected:1;
    llChar textInfo;
    uint16_t jumpPage;
    uint8_t keyValue;
    uint8_t disType;
    llColor pressedColor;
    llColor releasedColor;
    uint32_t pressedImageAddr;
    uint32_t releasedImageAddr;
    uint8_t buzzerCfg;
}llButton;

llButton *llButtonQuickCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,
                            uint8_t *text,llFontLib *fontLib,llColor textColor,
                            llColor bgReleasedColor,llColor bgRressedColor,uint32_t bgReleasedImageAddr,uint32_t bgPressedImageAddr,
                            uint8_t disType,bool isHidden);
llButton *llButtonCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height, uint8_t *text, llFontLib *fontLib,bool isHidden);

void pButtonSetGeometry(llButton *widget,int16_t x,int16_t y,int16_t width,int16_t height);
void nButtonSetGeometry(uint16_t nameId, int16_t x, int16_t y, int16_t width, int16_t height);

void pButtonSetFont(llButton *widget,llFontLib *fontLib);
void nButtonSetFont(uint16_t nameId, llFontLib *fontLib);

void pButtonSetTextColor(llButton *widget,llColor color);
void nButtonSetTextColor(uint16_t nameId,llColor color);

void pButtonSetBackgroundColor(llButton *widget,llColor releasedColor,llColor pressedColor);
void nButtonSetBackgroundColor(uint16_t nameId,llColor releasedColor,llColor enableOnColor);

void pButtonSetBackgroundImage(llButton *widget,uint32_t releasedImageAddr,uint32_t pressedImageAddr);
void nButtonSetBackgroundImage(uint16_t nameId,uint32_t releasedImageAddr,uint32_t pressedImageAddr);

void pButtonSetStyle(llButton *widget);
void nButtonSetStyle(uint16_t nameId);
    
void nButtonSetJumpPage(uint16_t nameId, uint16_t targetPageNum);

void pButtonSetText(llButton *widget, uint8_t *text);
void nButtonSetText(uint16_t nameId,uint8_t *text);

uint8_t* pButtonGetText(llButton *widget);
uint8_t* nButtonGetText(uint16_t nameId);

void pButtonSetEnabled(llButton *widget, bool state);
void nButtonSetEnabled(uint16_t nameId,bool state);

bool pButtonGetEnabled(llButton *widget);
bool nButtonGetEnabled(uint16_t nameId);

void pButtonSetCheckable(llButton *widget, bool state);
void nButtonSetCheckable(uint16_t nameId,bool state);

bool pButtonGetCheckable(llButton *widget);
bool nButtonGetCheckable(uint16_t nameId);

void pButtonSetChecked(llButton *widget, bool state);
void nButtonSetChecked(uint16_t nameId,bool state);

bool pButtonGetChecked(llButton *widget);
bool nButtonGetChecked(uint16_t nameId);

bool pButtonGetPressed(llButton *widget);
bool nButtonGetPressed(uint16_t nameId);

void pButtonSetPreselected(llButton *widget, bool state);
void nButtonSetPreselected(uint16_t nameId,bool state);

bool pButtonGetPreselected(llButton *widget);
bool nButtonGetPreselected(uint16_t nameId);

void pButtonToggle(llButton *widget);
void nButtonToggle(uint16_t nameId);

void pButtonClick(llButton *widget);
void nButtonClick(uint16_t nameId);

void pButtonRefresh(llButton *widget,bool toggle);
void nButtonRefresh(uint16_t nameId);

void pButtonSetKeyValue(llButton *widget,uint8_t value);
void nButtonSetKeyValue(uint16_t nameId,uint8_t value);

uint8_t pButtonGetKeyValue(llButton *widget);
uint8_t nButtonGetKeyValue(uint16_t nameId);

void pButtonSetHidden(llButton *widget,bool isHidden);
void nButtonSetHidden(uint16_t nameId,bool isHidden);

void pButtonSetBuzzerCfg(llButton *widget,uint8_t cfg);
void nButtonSetBuzzerCfg(uint16_t nameId,uint8_t cfg);

void pButtonMove(llButton *widget,int16_t x, int16_t y);
void nButtonMove(uint16_t nameId,int16_t x, int16_t y);

#ifdef __cplusplus
}
#endif

#endif //_LL_BUTTON_H_
