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

#ifndef _LL_FONT_H_
#define _LL_FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"


typedef struct font_info
{
    uint8_t* fontPath;
    uint32_t font_size;
    uint32_t buffer_size;
    uint8_t width_size;
    uint8_t height_size;
}Font_Info_TypeDef;

void llFontOpenLibrary(llFontLibType fontLibType, uint8_t* fontName, uint16_t fontSize);
uint8_t* llFontGetData(uint8_t * word, uint8_t *dataBits,uint16_t *width, uint16_t *height, bool *isMalloc);
uint8_t llFontGetWidthSize(uint8_t * word);
uint8_t llFontGetHeightSize(void);


#ifdef __cplusplus
}
#endif

#endif //_LL_FONT_H_
