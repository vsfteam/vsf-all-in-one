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

#ifndef _LL_GAUGE_H_
#define _LL_GAUGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"
#include "LL_Handler.h"



typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isColor:1;
    bool isBuffer1First:1;
    llColor backgroundColor;
    llColor pointerColor;
    uint32_t imageAddr;
    int16_t rotaryAngle;
    int16_t angleBuffer;
    uint16_t pointerLength;
    uint8_t pointerWidth1;
    uint8_t pointerWidth2;
    llPoint originalPoint;
    int16_t originalPointerDistance;
    llPoint *pointBackup;
    uint32_t pointCount;

    llPointColor *pointColorBuffer1;
    uint32_t pointColorCount1;
    llPointColor *pointColorBuffer2;
    uint32_t pointColorCount2;

    uint32_t loopCounter;
//    llPoint drawEndpointBackup[4];
}llGauge;


llGauge *llGaugeCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x, uint16_t y,uint32_t imageAddr,bool isColor,llColor backgroundColor,int16_t originalPointX,int16_t originalPointY,int16_t originalPointerDistance,uint16_t pointerLength,uint8_t pointerWidth1,uint8_t pointerWidth2,llColor pointerColor,bool isHidden);
void pGaugeSetRotaryAngle(llGauge *widget,int16_t rotaryAngle,bool isInit);
void nGaugeSetRotaryAngle(uint16_t nameId,int16_t rotaryAngle,bool isInit);
int16_t pGaugeGetRotaryAngle(llGauge *widget);
int16_t nGaugeGetRotaryAngle(uint16_t nameId);
void pGaugeLoop(llGauge *widget,uint16_t refreshMs,uint8_t moveValue);
void nGaugeLoop(uint16_t nameId,uint16_t refreshMs,uint8_t moveValue);
void pGaugeSetEnabled(llGauge *widget, bool state);
void nGaugeSetEnabled(uint16_t nameId, bool state);
void pGaugeSetHidden(llGauge *widget,bool isHidden);
void nGaugeSetHidden(uint16_t nameId,bool isHidden);

#ifdef __cplusplus
}
#endif

#endif //_LL_GAUGE_H_
