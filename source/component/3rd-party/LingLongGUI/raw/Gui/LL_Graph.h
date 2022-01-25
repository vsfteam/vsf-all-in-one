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

#ifndef _LL_GRAPH_H_
#define _LL_GRAPH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isHideGrid:1;
    llColor backgroundColor;
    llColor gridColor;//栅格颜色
    uint8_t horizontalInterval;//水平间隔
    uint8_t verticalInterval;//垂直间隔
    uint16_t pointSize;
    uint16_t pointCount;
    llPoint *pointBuff;
}llGraph;

llGraph *llGraphCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height, uint16_t pointSize,bool isHidden);

void nGraphRefresh(uint16_t nameId);

void pGraphAddPoint(llGraph *widget, int16_t x, int16_t y);
llGraph* llGraphGetWidget(uint16_t nameId);
void pGraphSetHidden(llGraph *widget,bool isHidden);
void nGraphSetHidden(uint16_t nameId,bool isHidden);
#ifdef __cplusplus
}
#endif

#endif //_LL_GRAPH_H_
