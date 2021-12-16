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

#ifndef _LL_CHARACTERS_H_
#define _LL_CHARACTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_User.h"
#include "LL_Font.h"
#include "LL_General.h"

typedef struct
{
    llGeometry geometry;
    llGeometry showGeometry;
    llColor charColor;
    llColor backgroundColor;

    bool isTransparent:1;
    bool isAutoLineBreak:1;
    bool isPassword:1;
    llHorizontalAlign hAlign:2;
    llVerticalAlign vAlign:2;

    llFontLib *fontLibInfo;
    uint8_t *text;
    uint16_t mallocLength;
    uint16_t textPointWidth;
}llChar;

llGeometry llCharDisplay(llChar* charInitStruct);
#define llCharOpenLibrary(type,name,size)             llFontOpenLibrary(type,name,size)
#define llCharCloseLibrary()                 llFontCloseLibrary()
#define llCharGetWidthSize(p)                llFontGetWidthSize(p)
int16_t llCharCalVerticalPos(llChar* charInitStruct, int16_t textHeight);
void llCharShowChar(int16_t x, int16_t y,llGeometry showGeometry,uint8_t *str,llColor color);
llGeometry llCharGetCharPos(llChar* charInitStruct,uint8_t *str,uint16_t offsetStr);

void llCharDisplayCursor(llPoint pos, uint16_t height, llColor backgroundColor, bool *isShow);
void llCharCursorBlinkLoop(void);
void llCharSetCursor(void* blinkWidget, bool *enableFlag, int16_t x, int16_t y, uint16_t cursorBlinkHeight, llColor backgroundColor);
void llCharAutoStopCursorBlink(void* clickWidget);
void llCharStopCursorBlink(void);
#ifdef __cplusplus
}
#endif

#endif //_LL_CHARACTERS_H_
