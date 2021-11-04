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

#ifndef _LL_QRCODE_H_
#define _LL_QRCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"
#include "LL_Handler.h"



typedef struct
{
    LLGENERAL_ATTRIBUTES;
    uint8_t* qrText;
    uint8_t qrEcc;
    uint8_t qrMask;
    uint8_t qrMaxVersion;
    uint8_t qrZoom;
    llColor bgColor;
    llColor qrColor;
}llQRCode;

llQRCode *llQRCodeQuickCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x,uint16_t y,
                                uint8_t* qrText,llColor qrColor,llColor bgColor,
                                uint8_t qrEcc,uint8_t qrMask,uint8_t qrMaxVersion,uint8_t qrZoom,
                                bool isHidden);
llQRCode *llQRCodeCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x,uint16_t y,uint8_t* qrText,uint8_t qrEcc,uint8_t qrMask,uint8_t qrMaxVersion,uint8_t qrZoom,bool isHidden);
void pQRCodeSetEnabled(llQRCode *widget, bool state);
void nQRCodeSetEnabled(uint16_t nameId, bool state);
void pQRCodeSetText(llQRCode *widget, uint8_t *text);
void nQRCodeSetText(uint16_t nameId, uint8_t *text);
uint8_t* pQRCodeGetText(llQRCode *widget);
uint8_t* nQRCodeGetText(uint16_t nameId);
void pQRCodeSetHidden(llQRCode *widget,bool isHidden);
void nQRCodeSetHidden(uint16_t nameId,bool isHidden);
#ifdef __cplusplus
}
#endif

#endif //_LL_QRCODE_H_
