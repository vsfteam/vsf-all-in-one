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

#ifndef _LL_LATTICE_H_
#define _LL_LATTICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"

//typedef struct
//{
//    uint16_t unicodeH;
//    uint16_t unicodeL;
//    uint32_t width;
//    uint32_t height;
//    void * dataPointer;
//}llLatticeInfo;

//typedef struct
//{
//    llLatticeInfo *dataInfo;
//    uint32_t totalCharNum;
//    uint8_t charSize;
//}llLatticeLibraryInfo;

void llLattice_fontOpenLibrary(llLatticeLibraryInfo * libPointer);
void llLattice_fontCloseLibrary(void);
void llLattice_fontGetData(uint8_t *utf8In, uint8_t *dataBits, void **retBitmap, uint16_t *retWidth, uint16_t *retHeight);
uint8_t llLattice_fontGetWidthSize(uint8_t * utf8In);
uint8_t llLattice_fontGetHeightSize(void);





#ifdef __cplusplus
}
#endif

#endif //_LL_LATTICE_H_
