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

#ifndef _LL_CONFIG_H_
#define _LL_CONFIG_H_

#include <stdlib.h>
#include "freeRtosHeap4.h"
#include "w25qxx.h"
#include "stdbool.h"
#include "setDefine.h"

extern uint8_t touchType;

#define TOUCH_TYPE_GT911											0
#define TOUCH_TYPE_NS2009											1

/***********************************控件内存占用定义*********************************************/

#define LLFONT_NAME_LENGTH_MAX                    48 //byte (修改无效)字体名称长度最大值

#define LLBUTTON_TEXT_LENGTH_MAX                  16 //byte button显示文本最大值

#define LLLINEEDIT_TEXT_LENGTH_MAX                48 //byte lineEdit显示文本最大值

#define LLCHECKBOX_TEXT_LENGTH_MAX                16 //byte checkBox显示文本最大值

extern uint8_t cfgButtonTextLengthMax;
extern uint8_t cfgLineEditTextLengthMax;
extern uint8_t cfgCheckBoxTextLengthMax;

/***********************************屏幕参数定义*********************************************/
////颜色位数
#define CONFIG_COLOR_DEPTH                        16 // 1 8 16 24 32
//屏幕宽度像素
#define CONFIG_MONITOR_WIDTH                      LCD_WIDTH
//屏幕高度像素
#define CONFIG_MONITOR_HEIGHT                     LCD_HEIGHT

extern uint8_t cfgColorDepth;
extern uint16_t cfgMonitorWidth;
extern uint16_t cfgMonitorHeight;

#define llColor  uint16_t
//#if CONFIG_COLOR_DEPTH == 1
//#define llColor  uint8_t
//#elif CONFIG_COLOR_DEPTH == 8
//#define llColor  uint8_t
//#elif CONFIG_COLOR_DEPTH == 16
//#define llColor  uint16_t
//#elif CONFIG_COLOR_DEPTH == 24
//#define llColor  uint32_t
//#elif CONFIG_COLOR_DEPTH == 32
//#define llColor  uint32_t
//#endif

/***********************************内存定义*********************************************/

#define MEM_SIZE                     (32*1024)

void *llMalloc(uint32_t size);
void llFree(void *p);
void *llRealloc(void *ptr,uint32_t newSize);

//双缓冲
#define USE_DOUBLE_BUFFERING                     1


#if USE_DOUBLE_BUFFERING == 1
extern uint32_t *lcdFrontBuf;
extern uint32_t *lcdBackBuf;

#define LL_LCD_BUF1_POINTER      LCD_RAM_BUFFER1
#define LL_LCD_BUF2_POINTER      LCD_RAM_BUFFER2
#endif

void llCfgLcdCopyFront2Back(void);
void llCfgSetLcdBufAddr(uint32_t *addr);
void llCfgSetLcdSrcAddr(uint32_t *addr);

//图片显示的缓冲大小
#define IMAGE_READ_BYTE_MAX      1024

/***********************************调试输出*********************************************/

//#define LLPRINT                                   //printf
//#define LLPRINT_NUM(str,num)                      //printf("%s:%d\n",str,num)
//#define LLPRINT_STR(str)                          //printf("%s\n",str)
//#define LLPRINT_POINT(str,pos)                    //printf("%s:%d,%d\n",str,pos.x,pos.y)
//#define LLPRINT_GEOMETRY(str,geometry)            //printf("%s:%d,%d,%d,%d\n",str,geometry.x,geometry.y,geometry.width,geometry.height)

/***********************************屏幕驱动*********************************************/

#define USE_USER_FILL_MULTIPLE_COLORS               1
//左上角为0,0
void llCfgSetPoint(int16_t x,int16_t y,llColor color);
llColor llCfgGetPoint(int16_t x,int16_t y);
void llCfgFillSingleColor(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color);
#if USE_USER_FILL_MULTIPLE_COLORS == 1
void llCfgFillMultipleColors(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor *color);
#endif
//void llCfgDrawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color);

/***********************************触摸驱动*********************************************/
bool llCfgClickGetPoint(int16_t *x,int16_t *y);



/***********************************定时器*********************************************/

// 开定时器
// 调用头文件 #include "LL_Timer.h"
// 定时器周期性运行 llTimer_ticks(ms);

/***********************************外部储存接口*********************************************/
void llExFlashInit(void);
void llReadExFlash(uint32_t addr,uint8_t* pBuffer,uint16_t length);

/***********************************蜂鸣器接口*********************************************/
void llBuzzerBeep(void);

/***********************************RTC接口*********************************************/
void llGetRtc(uint8_t *readBuf);
void llSetRtc(uint8_t *writeBuf);

#endif //_LL_CONFIG_H_
