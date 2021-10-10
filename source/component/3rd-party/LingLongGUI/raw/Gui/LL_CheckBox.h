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

#ifndef _LL_CHECKBOX_H_
#define _LL_CHECKBOX_H_


#include "LL_General.h"
#include "LL_Characters.h"




typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isChecked:1;
    bool isSelected:1;
    llChar textInfo;
    uint8_t * checkImagePath;
    uint8_t * noCheckImagePath;
}llCheckBox;

llCheckBox *llCheckBoxQuickCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,
                                uint8_t *text,llFontLib *fontLib,llColor textColor,
                                bool isChecked,bool isHidden);
llCheckBox *llCheckBoxCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y , int16_t width, int16_t height , uint8_t *text, llFontLib *fontLib,bool isHidden);

void pCheckBoxRefresh(llCheckBox *widget,bool toggle);
void nCheckBoxRefresh(uint16_t nameId);

bool pCheckBoxGetChecked(llCheckBox *widget);
bool nCheckBoxGetChecked(uint16_t nameId);
void pCheckBoxSetChecked(llCheckBox *widget, bool state);
void nCheckBoxSetChecked(uint16_t nameId, bool state);
void pCheckBoxSetEnabled(llCheckBox *widget, bool state);
void nCheckBoxSetEnabled(uint16_t nameId, bool state);
void pCheckBoxSetHidden(llCheckBox *widget,bool isHidden);
void nCheckBoxSetHidden(uint16_t nameId,bool isHidden);
#endif //_LL_CHECKBOX_H_
