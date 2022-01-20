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

#include "LL_Config.h"
#include "string.h"
#include "freeRtosHeap4.h"

uint8_t cfgButtonTextLengthMax = LLBUTTON_TEXT_LENGTH_MAX;
uint8_t cfgLineEditTextLengthMax = LLLINEEDIT_TEXT_LENGTH_MAX;
uint8_t cfgCheckBoxTextLengthMax = LLCHECKBOX_TEXT_LENGTH_MAX;

uint8_t cfgColorDepth = CONFIG_COLOR_DEPTH;
uint16_t cfgMonitorWidth = 0;
uint16_t cfgMonitorHeight = 0;

#if USE_DOUBLE_BUFFERING == 1
uint32_t *lcdFrontBuf=LL_LCD_BUF1_POINTER;
uint32_t *lcdBackBuf=LL_LCD_BUF2_POINTER;
#endif

void llCfgSetLcdBufAddr(uint32_t *addr)
{
#if USE_DOUBLE_BUFFERING == 1
    lcdSetBufferAddr(addr);
#endif
}

void llCfgSetLcdSrcAddr(uint32_t *addr)
{
#if USE_DOUBLE_BUFFERING == 1
    lcdSetSrcAddr(addr);
#endif
}

void llCfgLcdCopyFront2Back(void)
{
#if USE_DOUBLE_BUFFERING == 1
//    memcpy(lcdBackBuf,lcdFrontBuf,SDRAM_LCD_SIZE);
    uint64_t i;
    for(i=0;i<SDRAM_LCD_SIZE;i++)
    {
        lcdBackBuf[i]=lcdFrontBuf[i];
    }
#endif
}

bool llCfgClickGetPoint(int16_t *x,int16_t *y)
{
    bool ret;
    return ret;
}

void llCfgSetPoint(int16_t x,int16_t y,llColor color)
{
}

llColor llCfgGetPoint(int16_t x,int16_t y)
{
    llColor retColor;
    return retColor;
}

void llCfgFillSingleColor(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color)
{
}


void *llMalloc(uint32_t size)
{
    return malloc(size);
}

void llFree(void *p)
{
    free(p);
    p=NULL;
}

void *llRealloc(void *ptr,uint32_t newSize)
{
    return realloc(ptr,newSize);
}

void llExFlashInit(void)
{
}

void llReadExFlash(uint32_t addr,uint8_t* pBuffer,uint16_t length)
{
}

void llBuzzerBeep(void)
{
}

/***************************************************************************//**
 * @fn         void llGetRtc(uint8_t *readBuf)
 * @brief      读取年月日时分秒周
 * @param      *readBuf yy yy mm dd hh mm ss ww
 * @return     void
 * @version    V0.1
 * @date       
 * @details    数据用16进制储存,2021年 yyyy=0x07E5
 ******************************************************************************/
void llGetRtc(uint8_t *readBuf)
{
}

/***************************************************************************//**
 * @fn         void llSetRtc(uint8_t *writeBuf)
 * @brief      写入年月日时分秒
 * @param      *writeBuf yy yy mm dd hh mm ss
 * @return     void
 * @version    V0.1
 * @date       
 * @details    数据用16进制储存,2021年 yyyy=0x07E5
 ******************************************************************************/
void llSetRtc(uint8_t *writeBuf)
{
}

