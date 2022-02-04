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

#ifndef _LL_GENERAL_H_
#define _LL_GENERAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_Config.h"
#include "LL_Port.h"
#include <string.h>
#include "stdbool.h"
#include "LL_String.h"

#define ON           true
#define OFF          false
#define TOUCH_CLICK          true
#define TOUCH_NO_CLICK         false

#define   SET_BITS(data,value)          ((data)|=(value))
#define   CLR_BITS(data,value)          ((data)&=~(value))

#define   SETBIT(data,move)          ((data)|=(1<<(move)))
#define   CLRBIT(data,move)          ((data)&=~(1<<(move)))
//#define   GETBIT(data,move)          (((data)&(1<<(move)))?1:0)
#define   GETBIT(data,move)          (((data)>>(move))&0x01)

#define   PUTBIT(data,value,move)    ((value)?SETBIT(data,move):CLRBIT(data,move))

#define   GET16H(data)              (((data)>>8)&0xFF)
#define   GET16L(data)              ((data)&0xFF)
#define   CONNECT16(H,L)            (((H)<<8)+(L))

#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))

#define RECTANGLE_MID_X_POS(x,w1,w2)   (x+(w1-w2)/2)
#define RECTANGLE_MID_Y_POS(y,h1,h2)   (y+(h1-h2)/2)
#define RECTANGLE_RIGHT_X_POS(x,w1,w2)   (x+(w1-w2))
#define RECTANGLE_BUTTOM_Y_POS(y,h1,h2)   (y+(h1-h2))

#define IMAGE_NONE    0xFFFFFFFF
typedef enum{
    widgetTypeNone,//仅background使用
    widgetTypeBackground,
    widgetTypeWindow,
    widgetTypeButton,
    widgetTypeImage,
    widgetTypeText,
    widgetTypeLineEdit,
    widgetTypeDashboardSimple,
    widgetTypeGraph,
    widgetTypeCheckBox,
    widgetTypeSlider,
    widgetTypeProgressBar,
    widgetTypeGauge,
    widgetTypeQRCode,
    widgetTypeDateTime,
    widgetTypeIconSlider,
    widgetTypeComboBox
}llWidgetType;

#define LLGENERAL_ATTRIBUTES  llWidgetType widgetType; \
                             llGeometry geometry; \
                             uint16_t nameId; \
                             llWidgetType parentType; \
                             void * parentWidget; \
                             void (*deleteFunc)(uint16_t); \
                             void (*actionFunc)(void *,uint8_t); \
                             void (*refreshFunc)(uint16_t); \
                             bool isEnable:1; \
                             bool isHidden:1

typedef struct{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
}llGeometry;

typedef struct{
    LLGENERAL_ATTRIBUTES;
}llGeneral;

typedef struct{
    int16_t x;
    int16_t y;
}llPoint;

typedef struct
{
    int16_t x;
    int16_t y;
    llColor color;
}llPointColor;

typedef struct{
    int16_t width;
    int16_t height;
}llSize;

typedef enum
{
    llAlignHLeft,
    llAlignHCenter,
    llAlignHRight
}llHorizontalAlign;

typedef enum
{
    llAlignVTop,
    llAlignVCenter,
    llAlignVBottom
}llVerticalAlign;

typedef enum
{
    typeTTF,
    typeCFile,
    typeDataFile
}llFontLibType;


typedef struct
{
    llFontLibType libType;
    uint8_t name[LL_FONT_NAME_LENGTH_MAX];
    uint16_t fontSize;
    void * pointer;
}llFontLib;

typedef struct
{
    uint16_t unicodeH;
    uint16_t unicodeL;
    uint32_t width;
    uint32_t height;
    void * dataPointer;
}llLatticeInfo;

typedef struct
{
    const llLatticeInfo *dataInfo;
    uint8_t dataBits;
    uint32_t totalCharNum;
    uint8_t charSize;
}llLatticeLibraryInfo;

#define LL_MALLOC_WIDGET_INFO(widget_type)      (widget_type*)llMalloc(sizeof(widget_type))

#define LL_MALLOC_STRING(str)      (uint8_t *)llMalloc((strlen((const char *)str)+1)*sizeof(uint8_t))

// 1 8(233) 16(565) 24(888)
#if CONFIG_COLOR_DEPTH == 1

#elif CONFIG_COLOR_DEPTH == 8

#elif CONFIG_COLOR_DEPTH == 16
uint16_t llRgb565ToGray(uint16_t rgb565);
#define RGB_2_GRAY(color)        llRgb565ToGray(color)
#elif CONFIG_COLOR_DEPTH == 24
uint32_t llRgb888ToGray(uint32_t rgb888);
#define RGB_2_GRAY(color)        llRgb888ToGray(color)
#endif

uint8_t llUtf8ToUnicode(uint8_t* unicode, uint8_t *utf8);
bool llStringCompare(uint8_t* str1,uint8_t* str2);
uint8_t* llStrcat(uint8_t* str1,uint8_t* str2);
bool llRectIsOverlap(llGeometry rc1, llGeometry rc2);
bool llRectIsFullIn(llGeometry bigRect, llGeometry smallRect);
bool llPointInRect(llPoint point, llGeometry rc);
bool llRectIntersect(const llGeometry rect1, const llGeometry rect2,llGeometry * outRect);
//bool llPointInRect(int16_t x, int16_t y,llPoint *buf,uint32_t bufCount);
llGeometry llGeneralImageShow(llGeneral *widget,uint32_t imageAddr, int16_t x, int16_t y);
llSize llGeneralGetImageSize(uint32_t imageAddr);
llGeometry llGeneralGetImageGeometry(int16_t x,int16_t y,uint32_t imageAddr);
void llGeneralImageSpecificAreaShow(llGeneral *widget,llGeometry originGeometry,llGeometry targetGeometry,uint32_t imageAddr);
//void llRgbaFillMultipleColors(llGeometry geometry, uint8_t *color);
//void llRgbFillMultipleColors(llGeometry geometry,uint8_t *color);
//void llRgbaFillSpecificAreaMultipleColors(llGeometry originGeometry,llGeometry targetGeometry, uint8_t *color);
//void llRgbFillSpecificAreaMultipleColors(llGeometry originGeometry,llGeometry targetGeometry, uint8_t *color);
//llPointColor llGeneralGetColor(int16_t x,int16_t y);
//void llGeneralRectBackupPointColor(llPointColor* outBuf,llGeometry geometry);
void llGeneralRectBackupColor(llColor* outBuf,llGeometry geometry);

void* llGeneralGetWidget(uint16_t nameId, llWidgetType widgetType);
void llGeneralWidgetSelect(llGeneral *widget);
void llGeneralWidgetParentRecover(llGeneral *widget,llGeometry geometry);
bool llGeneralParentLinkHidden(llGeneral *widget);
void llGeneralWidgetParentRecoverMove(llGeneral *widget,llGeometry oldGeometry,llGeometry newGeometry);
void llGeneralRecover(llGeneral *widget,llGeometry targetGeometry);

#define PARENT_BACKGROUND_TYPE_ERROR   0
#define PARENT_BACKGROUND_TYPE_COLOR   1
#define PARENT_BACKGROUND_TYPE_IMAGE   2

uint8_t llGeneralGetParentBackgroundInfo(llGeneral *widget,llColor* retColor,uint32_t *retImageAddr,llPoint* retImagePoint);

/***********************************888转换屏幕位数颜色*********************************************/

llColor RGB888(uint32_t color);
llColor RGB_CONVERT(uint8_t R,uint8_t G,uint8_t B);
void llGeometryGetR_G_B(llColor color,uint8_t *r,uint8_t *g,uint8_t *b);
bool llGeometryAutoSetHiddenIsShow(llGeneral *widget,bool isHidden);
llColor llGeometryColorMix(llColor rgb1,llColor rgb2,uint8_t alpha2);

#ifdef __cplusplus
}
#endif

#endif //_LL_GENERAL_H_
