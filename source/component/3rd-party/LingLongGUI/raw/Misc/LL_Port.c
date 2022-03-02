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

#include "LL_Port.h"
#include "string.h"
#include "LL_User.h"

#define MIRROR_X(x,w) ((x)=(w-1)-(x))
#define MIRROR_Y(y,h) ((y)=(h-1)-(y))
#define SWAP(x,y) ((x) = (x)+(y),(y)=(x)-(y),(x)=(x)-(y))


#define ROTATE0_XY(x,y)
#define ROTATE0_RECT(x0,y0,x1,y1)

#define ROTATE90_XY(x,y)                  SWAP((x),(y));MIRROR_X((x),CONFIG_MONITOR_WIDTH)
#define ROTATE90_RECT(x0,y0,x1,y1)        SWAP(x0,y0);MIRROR_X(x0,CONFIG_MONITOR_WIDTH);SWAP(x1,y1);MIRROR_X(x1,CONFIG_MONITOR_WIDTH);SWAP(x0,x1)

#define ROTATE180_XY(x,y)                  MIRROR_X((x),CONFIG_MONITOR_WIDTH);MIRROR_Y((y),CONFIG_MONITOR_HEIGHT)
#define ROTATE180_RECT(x0,y0,x1,y1)        MIRROR_X(x0,CONFIG_MONITOR_WIDTH);MIRROR_Y(y0,CONFIG_MONITOR_HEIGHT);MIRROR_X(x1,CONFIG_MONITOR_WIDTH);MIRROR_Y(y1,CONFIG_MONITOR_HEIGHT);SWAP(x0,x1);SWAP(y0,y1)

#define ROTATE270_XY(x,y)                  SWAP((x),(y));MIRROR_Y((y),CONFIG_MONITOR_HEIGHT)
#define ROTATE270_RECT(x0,y0,x1,y1)        SWAP(x0,y0);MIRROR_Y(y0,CONFIG_MONITOR_HEIGHT);SWAP(x1,y1);MIRROR_Y(y1,CONFIG_MONITOR_HEIGHT);SWAP(y0,y1)

static bool isDoubleBufferCopyEnd=false;
static bool isDoubleBufferWaitExchangeRefresh=false;
void llDoubleBufferStart(void)
{
#if USE_DOUBLE_BUFFERING == 1
    //判断
    if(isDoubleBufferCopyEnd==false)
    {
        //全屏已复制标记
        isDoubleBufferCopyEnd=true;
        //复制
        llCfgLcdCopyFront2Back();
    }
    
    
    // 切换地址 back
    llCfgSetLcdBufAddr(lcdBackBuf);
#endif
}

void llDoubleBufferEnd(bool isRefreshNow)
{
#if USE_DOUBLE_BUFFERING == 1
    // 切换地址 front
    llCfgSetLcdBufAddr(lcdFrontBuf);
    if(isRefreshNow)
    {
        isDoubleBufferWaitExchangeRefresh=true;
//        while(isDoubleBufferWaitExchangeRefresh);
        llDoubleBufferClear();
    }
#endif
}

void llDoubleBufferClear(void)
{
#if USE_DOUBLE_BUFFERING == 1
    uint32_t *pLcdBuf;
    if(isDoubleBufferWaitExchangeRefresh==true)
    {
    //全屏已复制标记 清除
    isDoubleBufferCopyEnd=false;
    //刷新标记 清除
    isDoubleBufferWaitExchangeRefresh=false;
    //交换buf
    pLcdBuf=lcdFrontBuf;
    lcdFrontBuf=lcdBackBuf;
    lcdBackBuf=pLcdBuf;
    llCfgSetLcdBufAddr(lcdFrontBuf);
        llCfgSetLcdSrcAddr(lcdFrontBuf);
    }
#endif
}

void llDoubleBufferRefresh(void)
{
    if(isDoubleBufferCopyEnd==true)
    {
        //刷新 标记
        isDoubleBufferWaitExchangeRefresh=true;
    }
}

static void lcdRotateXY(uint16_t lcdAngle,int16_t *x,int16_t *y)
{
    switch(lcdAngle)
    {
        case 0:
        {
            ROTATE0_XY(*x,*y);
            break;
        }
        case 90:
        {
            ROTATE90_XY(*x,*y);
            break;
        }
        case 180:
        {
            ROTATE180_XY(*x,*y);
            break;
        }
        case 270:
        {
            ROTATE270_XY(*x,*y);
            break;
        }
        default:
            break;
    }
}

static void lcdRotateRect(uint16_t lcdAngle,int16_t *x0,int16_t *y0,int16_t *x1,int16_t *y1)
{
    switch(lcdAngle)
    {
        case 0:
        {
            ROTATE0_RECT(*x0,*y0,*x1,*y1);
            break;
        }
        case 90:
        {
            ROTATE90_RECT(*x0,*y0,*x1,*y1);
            break;
        }
        case 180:
        {
            ROTATE180_RECT(*x0,*y0,*x1,*y1);
            break;
        }
        case 270:
        {
            ROTATE270_RECT(*x0,*y0,*x1,*y1);
            break;
        }
        default:
            break;
    }
}

bool llClickGetPoint(int16_t *x,int16_t *y)
{
    bool touchState=false;
    
	touchState=llCfgClickGetPoint(x,y);
    lcdRotateXY(llUserGetAngle(),x,y);
    
    return touchState;
}

void llSetPoint(int16_t x,int16_t y,llColor color)
{
    lcdRotateXY(llUserGetAngle(),&x,&y);
    llCfgSetPoint(x,y,color);
}

llColor llGetPoint(int16_t x,int16_t y)
{
    lcdRotateXY(llUserGetAngle(),&x,&y);
    return llCfgGetPoint(x,y);
}

void llFillSingleColor(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color)
{
    lcdRotateRect(llUserGetAngle(),&x0,&y0,&x1,&y1);
    llCfgFillSingleColor(x0,y0,x1,y1,color);
}

void llFillMultipleColors(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor *color)
{
#if USE_USER_FILL_MULTIPLE_COLORS == 1
    llCfgFillMultipleColors(x0,y0,x1,y1,color);
#else
    uint16_t height,width;
    uint16_t i,j;
    width=x1-x0+1;
    height=y1-y0+1;
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
        {
            llSetPoint(x0+j,y0+i,color[i*width+j]);
        }
    }
#endif
}

void llDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, llColor color)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = abs(x1 - x0);        /* The difference between the x's */
    deltay = abs(y1 - y0);        /* The difference between the y's */
    x = x0;                       /* Start x off at the first pixel */
    y = y0;                       /* Start y off at the first pixel */

    if (x1 >= x0)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y1 >= y0)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        llSetPoint(x, y,color);             /* Draw the current pixel */
        num += numadd;              /* Increase the numerator by the top of the fraction */
        if (num >= den)             /* Check if numerator >= denominator */
        {
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }
}






