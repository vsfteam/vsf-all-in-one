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

#ifndef _LL_LINEEDIT_H_
#define _LL_LINEEDIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"


#include "LL_Characters.h"


#define KEYBOARD_NONE    0
#define KEYBOARD_NUM     1
#define KEYBOARD_QWERTY  2
#define KEYBOARD_USER    3
typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isOnlyInputNum:1;
    bool isCursorEnable:1;
    bool isNum:1;
    llChar textInfo;
    bool isInput;//此变量不可压缩
    llFontLib *keyboardFontLib;
    float minValue;
    float maxValue;
    uint16_t textMaxLength;
    uint8_t keyboardType;
}llLineEdit;

llLineEdit *llLineEditQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                uint8_t *text,llFontLib *fontLib,llColor textColor,
                                llFontLib *keyboardFontLib,
                                bool isHidden);
llLineEdit *llLineEditCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height, uint8_t *text, llFontLib *fontLib,llFontLib *keyboardFontLib,bool isHidden);
void pLineEditSetText(llLineEdit *widget,uint8_t *text);
void nLineEditSetText(uint16_t nameId,uint8_t *text);
uint8_t* pLineEditGetText(llLineEdit *widget);
uint8_t* nLineEditGetText(uint16_t nameId);

void pLineEditSetSelect(llLineEdit *widget,bool state);
void nLineEditSetSelect(uint16_t nameId,bool state);
void pLineEditSetEnabled(llLineEdit *widget, bool state);
void nLineEditSetEnabled(uint16_t nameId, bool state);
void pLineEditSetHidden(llLineEdit *widget,bool isHidden);
void nLineEditSetHidden(uint16_t nameId,bool isHidden);
void pLineEditSetCursorEnable(llLineEdit *widget,bool state);
void nLineEditSetCursorEnable(uint16_t nameId,bool state);
void nLineEditSetKeyboard(uint16_t nameId,uint8_t keyboardType);
void pLineEditSetMinNum(llLineEdit *widget,float value);
void nLineEditSetMinNum(uint16_t nameId,float value);
void pLineEditSetMaxNum(llLineEdit *widget,float value);
void nLineEditSetMaxNum(uint16_t nameId,float value);
void pLineEditPasswordEnable(llLineEdit *widget,bool enable);
void nLineEditPasswordEnable(uint16_t nameId,bool enable);
#ifdef __cplusplus
}
#endif

#endif //_LL_LINEEDIT_H_
