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

#ifndef _LL_WINDOW_H_
#define _LL_WINDOW_H_

#include "LL_General.h"

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isTransparent:1;
    llColor color;
    void * userInfo;
}llWindow;

llWindow *llWindowQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                llColor bgColor,
                                bool isTransparent,bool isHidden);
llWindow *llWindowCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height,bool isHidden);

void pWindowSetColor(llWindow *widget,llColor color);
void nWindowSetColor(uint16_t nameId,llColor color);

void pWindowSpecificAreaRecover(llWindow *widget,llGeometry geometry);
void pWindowSetEnabled(llWindow *widget, bool state);
void nWindowSetEnabled(uint16_t nameId, bool state);
//void pWindowBackgroundRecover(llWindow *widget);
void pWindowSetHiden(llWindow *widget,bool state);
void nWindowSetHiden(uint16_t nameId,bool state);

void nWindowDelete(uint16_t nameId);

#endif //_LL_WINDOW_H_
