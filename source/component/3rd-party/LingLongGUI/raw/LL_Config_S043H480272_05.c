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
#include "gt9xx.h"
#include "ns2009.h"
#include "lcd.h"
#include "freeRtosHeap4.h"
#include "w25qxx.h"
#include "buzzer.h"
#include "setDefine.h"
#include "bm8563.h"
#include "LL_User.h"
#include "iic.h"
uint8_t cfgButtonTextLengthMax = LLBUTTON_TEXT_LENGTH_MAX;
uint8_t cfgLineEditTextLengthMax = LLLINEEDIT_TEXT_LENGTH_MAX;
uint8_t cfgCheckBoxTextLengthMax = LLCHECKBOX_TEXT_LENGTH_MAX;

uint8_t cfgColorDepth = CONFIG_COLOR_DEPTH;
uint16_t cfgMonitorWidth = 0;
uint16_t cfgMonitorHeight = 0;

uint8_t touchType;
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
    bool touchState=false;
    uint16_t rx;
    uint16_t ry;
    if(touchType==TOUCH_TYPE_GT911)
    {
        touchState= gt9xx_scan(&rx,&ry);
    }
    else
    {
        touchState=ns2009_scan(0,&rx,&ry);
    }
    *x=rx;
    *y=ry;
    if((touchState!=0)&&(((rx!=0xffff)&&(ry!=0xffff))||((rx!=0)&&(ry!=0))))
    {
        touchState=true;
    }
    else
    {
        touchState=false;
        *x=0xffff;
        *y=0xffff;
    }
    return touchState;
}

void llCfgSetPoint(int16_t x,int16_t y,llColor color)
{
    lcdSetPoint(x,y,color);
}

llColor llCfgGetPoint(int16_t x,int16_t y)
{
    return lcdGetPoint(x,y);
}

void llCfgFillSingleColor(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color)
{
    lcdFill(x0,y0,x1,y1,color);
}

#if USE_USER_FILL_MULTIPLE_COLORS == 1
void llCfgFillMultipleColors(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor *color)
{
    uint16_t height,width;
    uint16_t i,j,n;
    width=x1-x0+1;
    height=y1-y0+1;
    for(i=0; i<height; i++)
    {
        j=0;
        if(x0%2)
        {
            llSetPoint(x0+j,y0+i,color[i*width+j]);
            j++;
        }
        n=(width-j)/2;
        while(n>0)
        {
            lcdSetMultiPoint(x0+j,y0+i,&color[i*width+j]);
            j+=2;
            n--;
        }
        n=(width-j)%2;
        if(n>0)
        {
            llSetPoint(x0+j,y0+i,color[i*width+j]);
        }
    }
}
#endif

void *llMalloc(uint32_t size)
{
    return pvPortMalloc(size);
}

void llFree(void *p)
{
    vPortFree(p);
    p=NULL;
}

void *llRealloc(void *ptr,uint32_t newSize)
{
    void *new_addr=NULL;
    if(ptr)
    {
        if(newSize!=0)
        {
            new_addr=llMalloc(newSize);

            if(new_addr!=NULL)
            {
                memcpy(new_addr,ptr,newSize);
                llFree(ptr);
            }
        }
    }
    else
    {
        if(newSize!=0)
        {
            new_addr=llMalloc(newSize);
        }
    }
    return new_addr;
}

void llExFlashInit(void)
{
        uint16_t i=0;
    uint8_t pBuffer[1024];
		uint8_t* pWrite=(uint8_t*)&sdramBuffer[SDRAM_NOR_ADDR];

    //自动复制nor到sdram
    for(i=0; i<1024; i++)
    {
        w25qxxRead(pBuffer,(i*1024),1024);
        //sdram_8bitWrite(SDRAM_NOR_ADDR+(i*1024),pBuffer,1024);
				sdram_8bitWrite(pWrite+(i*1024),pBuffer,1024);
    }
    
//    uint16_t i=0;
//    uint8_t pBuffer[1024];

//    //自动复制nor到sdram
//    for(i=0; i<1024; i++)
//    {
//        w25qxxRead(pBuffer,(i*1024),1024);
//        sdram_8bitWrite(SDRAM_NOR_ADDR+(i*1024),pBuffer,1024);
//    }
}

void llReadExFlash(uint32_t addr,uint8_t* pBuffer,uint16_t length)
{
//    w25qxxRead(pBuffer,addr,length);
//    sdram_8bitRead(SDRAM_NOR_ADDR+addr,pBuffer,length);
    
    uint8_t* pRead=(uint8_t*)&sdramBuffer[SDRAM_NOR_ADDR]+addr;
    //sdram_8bitRead(SDRAM_NOR_ADDR+addr,pBuffer,length);
		sdram_8bitRead(pRead,pBuffer,length);
}

void llBuzzerBeep(void)
{
    buzzerSetOn();
    llTimer_delayMs(50);
    buzzerSetOff();
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
    uint16_t year;
    uint8_t month,day,hour,minute,second,week;

    bm8563_readDate(&year,&month,&day);
    bm8563_readTime(&hour,&minute,&second);
    week= bm8563_getWeek(year,month,day);
    readBuf[0]=year>>8;
    readBuf[1]=year;
    readBuf[2]=month;
    readBuf[3]=day;
    readBuf[4]=hour;
    readBuf[5]=minute;
    readBuf[6]=second;
    readBuf[7]=week;
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
    bm8563_writeDate((writeBuf[0]<<8)+writeBuf[1],writeBuf[2],writeBuf[3]);
    bm8563_writeTime(writeBuf[4],writeBuf[5],writeBuf[6]);
}

