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

#include "LL_Gauge.h"
#include "LL_Linked_List.h"
#include "stdio.h"
#include "LL_Config.h"
#include "LL_Timer.h"

static const float sina_val[]= {
    0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f,
    0.139173f, 0.156434f, 0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f,
    0.275637f, 0.292372f, 0.309017f, 0.325568f, 0.342020f, 0.358368f, 0.374607f, 0.390731f,
    0.406737f, 0.422618f, 0.438371f, 0.453990f, 0.469472f, 0.484810f, 0.500000f, 0.515038f,
    0.529919f, 0.544639f, 0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615662f, 0.629320f,
    0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.719340f, 0.731354f,
    0.743145f, 0.754710f, 0.766044f, 0.777146f, 0.788011f, 0.798635f, 0.809017f, 0.819152f,
    0.829038f, 0.838671f, 0.848048f, 0.857167f, 0.866025f, 0.874620f, 0.882948f, 0.891007f,
    0.898794f, 0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f, 0.939693f, 0.945519f,
    0.951057f, 0.956305f, 0.961262f, 0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998630f,
    0.999391f, 0.999848f, 1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 0.996195f,
    0.994522f, 0.992546f, 0.990268f, 0.987688f, 0.984808f, 0.981627f, 0.978148f, 0.974370f,
    0.970296f, 0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 0.939693f, 0.933580f,
    0.927184f, 0.920505f, 0.913545f, 0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f,
    0.866025f, 0.857167f, 0.848048f, 0.838671f, 0.829038f, 0.819152f, 0.809017f, 0.798635f,
    0.788011f, 0.777146f, 0.766044f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 0.707107f,
    0.694658f, 0.681998f, 0.669131f, 0.656059f, 0.642788f, 0.629320f, 0.615662f, 0.601815f,
    0.587785f, 0.573576f, 0.559193f, 0.544639f, 0.529919f, 0.515038f, 0.500000f, 0.484810f,
    0.469472f, 0.453990f, 0.438371f, 0.422618f, 0.406737f, 0.390731f, 0.374607f, 0.358368f,
    0.342020f, 0.325568f, 0.309017f, 0.292372f, 0.275637f, 0.258819f, 0.241922f, 0.224951f,
    0.207912f, 0.190809f, 0.173648f, 0.156434f, 0.139173f, 0.121869f, 0.104528f, 0.087156f,
    0.069756f, 0.052336f, 0.034899f, 0.017452f, 0.000000f, -0.017452f, -0.034899f, -0.052336f,
    -0.069756f, -0.087156f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190809f,
    -0.207912f, -0.224951f, -0.241922f, -0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f,
    -0.342020f, -0.358368f, -0.374607f, -0.390731f, -0.406737f, -0.422618f, -0.438371f, -0.453990f,
    -0.469472f, -0.484810f, -0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, -0.573576f,
    -0.587785f, -0.601815f, -0.615662f, -0.629320f, -0.642788f, -0.656059f, -0.669131f, -0.681998f,
    -0.694658f, -0.707107f, -0.719340f, -0.731354f, -0.743145f, -0.754710f, -0.766044f, -0.777146f,
    -0.788011f, -0.798635f, -0.809017f, -0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f,
    -0.866025f, -0.874620f, -0.882948f, -0.891007f, -0.898794f, -0.906308f, -0.913545f, -0.920505f,
    -0.927184f, -0.933580f, -0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, -0.965926f,
    -0.970296f, -0.974370f, -0.978148f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f,
    -0.994522f, -0.996195f, -0.997564f, -0.998630f, -0.999391f, -0.999848f, -1.000000f, -0.999848f,
    -0.999391f, -0.998630f, -0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f,
    -0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, -0.965926f, -0.961262f, -0.956305f,
    -0.951057f, -0.945519f, -0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, -0.906308f,
    -0.898794f, -0.891007f, -0.882948f, -0.874620f, -0.866025f, -0.857167f, -0.848048f, -0.838671f,
    -0.829038f, -0.819152f, -0.809017f, -0.798635f, -0.788011f, -0.777146f, -0.766044f, -0.754710f,
    -0.743145f, -0.731354f, -0.719340f, -0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f,
    -0.642788f, -0.629320f, -0.615662f, -0.601815f, -0.587785f, -0.573576f, -0.559193f, -0.544639f,
    -0.529919f, -0.515038f, -0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, -0.422618f,
    -0.406737f, -0.390731f, -0.374607f, -0.358368f, -0.342020f, -0.325568f, -0.309017f, -0.292372f,
    -0.275637f, -0.258819f, -0.241922f, -0.224951f, -0.207912f, -0.190809f, -0.173648f, -0.156434f,
    -0.139173f, -0.121869f, -0.104528f, -0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f,
    0.000000f
};

static const float cosa_val[]= {
    1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 0.996195f, 0.994522f, 0.992546f,
    0.990268f, 0.987688f, 0.984808f, 0.981627f, 0.978148f, 0.974370f, 0.970296f, 0.965926f,
    0.961262f, 0.956305f, 0.951057f, 0.945519f, 0.939693f, 0.933580f, 0.927184f, 0.920505f,
    0.913545f, 0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f, 0.866025f, 0.857167f,
    0.848048f, 0.838671f, 0.829038f, 0.819152f, 0.809017f, 0.798635f, 0.788011f, 0.777146f,
    0.766044f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 0.707107f, 0.694658f, 0.681998f,
    0.669131f, 0.656059f, 0.642788f, 0.629320f, 0.615662f, 0.601815f, 0.587785f, 0.573576f,
    0.559193f, 0.544639f, 0.529919f, 0.515038f, 0.500000f, 0.484810f, 0.469472f, 0.453990f,
    0.438371f, 0.422618f, 0.406737f, 0.390731f, 0.374607f, 0.358368f, 0.342020f, 0.325568f,
    0.309017f, 0.292372f, 0.275637f, 0.258819f, 0.241922f, 0.224951f, 0.207912f, 0.190809f,
    0.173648f, 0.156434f, 0.139173f, 0.121869f, 0.104528f, 0.087156f, 0.069756f, 0.052336f,
    0.034899f, 0.017452f, 0.000000f, -0.017452f, -0.034899f, -0.052336f, -0.069756f, -0.087156f,
    -0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190809f, -0.207912f, -0.224951f,
    -0.241922f, -0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f, -0.342020f, -0.358368f,
    -0.374607f, -0.390731f, -0.406737f, -0.422618f, -0.438371f, -0.453990f, -0.469472f, -0.484810f,
    -0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, -0.573576f, -0.587785f, -0.601815f,
    -0.615662f, -0.629320f, -0.642788f, -0.656059f, -0.669131f, -0.681998f, -0.694658f, -0.707107f,
    -0.719340f, -0.731354f, -0.743145f, -0.754710f, -0.766044f, -0.777146f, -0.788011f, -0.798635f,
    -0.809017f, -0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f, -0.866025f, -0.874620f,
    -0.882948f, -0.891007f, -0.898794f, -0.906308f, -0.913545f, -0.920505f, -0.927184f, -0.933580f,
    -0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, -0.965926f, -0.970296f, -0.974370f,
    -0.978148f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f, -0.994522f, -0.996195f,
    -0.997564f, -0.998630f, -0.999391f, -0.999848f, -1.000000f, -0.999848f, -0.999391f, -0.998630f,
    -0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f, -0.984808f, -0.981627f,
    -0.978148f, -0.974370f, -0.970296f, -0.965926f, -0.961262f, -0.956305f, -0.951057f, -0.945519f,
    -0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, -0.906308f, -0.898794f, -0.891007f,
    -0.882948f, -0.874620f, -0.866025f, -0.857167f, -0.848048f, -0.838671f, -0.829038f, -0.819152f,
    -0.809017f, -0.798635f, -0.788011f, -0.777146f, -0.766044f, -0.754710f, -0.743145f, -0.731354f,
    -0.719340f, -0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f, -0.642788f, -0.629320f,
    -0.615662f, -0.601815f, -0.587785f, -0.573576f, -0.559193f, -0.544639f, -0.529919f, -0.515038f,
    -0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, -0.422618f, -0.406737f, -0.390731f,
    -0.374607f, -0.358368f, -0.342020f, -0.325568f, -0.309017f, -0.292372f, -0.275637f, -0.258819f,
    -0.241922f, -0.224951f, -0.207912f, -0.190809f, -0.173648f, -0.156434f, -0.139173f, -0.121869f,
    -0.104528f, -0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f, -0.000000f, 0.017452f,
    0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f, 0.139173f, 0.156434f,
    0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f, 0.275637f, 0.292372f,
    0.309017f, 0.325568f, 0.342020f, 0.358368f, 0.374607f, 0.390731f, 0.406737f, 0.422618f,
    0.438371f, 0.453990f, 0.469472f, 0.484810f, 0.500000f, 0.515038f, 0.529919f, 0.544639f,
    0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615662f, 0.629320f, 0.642788f, 0.656059f,
    0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.719340f, 0.731354f, 0.743145f, 0.754710f,
    0.766044f, 0.777146f, 0.788011f, 0.798635f, 0.809017f, 0.819152f, 0.829038f, 0.838671f,
    0.848048f, 0.857167f, 0.866025f, 0.874620f, 0.882948f, 0.891007f, 0.898794f, 0.906308f,
    0.913545f, 0.920505f, 0.927184f, 0.933580f, 0.939693f, 0.945519f, 0.951057f, 0.956305f,
    0.961262f, 0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f, 0.984808f, 0.987688f,
    0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998630f, 0.999391f, 0.999848f,
    1.000000f
};


void pGaugePointerRotary(llGauge *widget);


void llGaugeAction(void *widget,uint8_t touchSignal)
{

}

void pGaugeFree(llGauge *widget)
{
    llFree(widget->pointColorBuffer1);
    llFree(widget->pointColorBuffer2);
    llFree(widget->pointBackup);
    llFree(widget);
}

void nGaugeDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llGauge *widget;
    if(llList_GetInfoByName(&linkInfo,nameId)==true)
    {
        widget=linkInfo->widget;

        //查找父链表
        llList_GetInfoByName(&parentInfo,((llGeneral*)widget->parentWidget)->nameId);
        //消除自身在父链表中的位置数据
        list_for_each_prev_safe(tempPos,safePos, &parentInfo->child_link)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
            if(tempInfo->widget==widget)
            {
                llLinkedListDelete(tempPos);
                pGaugeFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void pGaugeRefresh(llGauge *widget)
{
    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            pGaugePointerRotary(widget);
        }
    }
}

void nGaugeRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGauge);
    if(widget!=NULL)
    {
        pGaugeRefresh((llGauge*)widget);
    }
}

bool pGaugeLoad(llGauge *widget, uint32_t imageAddr)
{
    llGeometry newGeometry;
    llPoint globalPos;
    if(widget->isEnable)
    {
        globalPos=llListGetGlobalPos(widget->parentWidget);
        llDoubleBufferStart();
        newGeometry=llGeneralImageShow((llGeneral*)widget,widget->imageAddr,widget->geometry.x+globalPos.x,widget->geometry.y+globalPos.y);
        llDoubleBufferEnd(true);
        widget->geometry.width=newGeometry.width;
        widget->geometry.height=newGeometry.height;
        return true;
    }
    else
    {
        return false;
    }
}

//自动根据图片大小更新尺寸数据
llGauge *llGaugeCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x,uint16_t y,uint32_t imageAddr,bool isColor,llColor backgroundColor,int16_t originalPointX,int16_t originalPointY,int16_t originalPointerDistance,uint16_t pointerLength,uint8_t pointerWidth1,uint8_t pointerWidth2,llColor pointerColor,bool isHidden)
{
    llGauge * pNewWidget = NULL;
    llPoint * pPointBackup = NULL;
    llPointColor *pPointColorBackup1 = NULL,*pPointColorBackup2 = NULL;
    uint32_t pointBufferSize=0;
    llListWidgetInfo *parentInfo;
    uint8_t widthTemp;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
    pNewWidget = LL_MALLOC_WIDGET_INFO(llGauge);

    widthTemp=(pointerWidth1>=pointerWidth2)?pointerWidth1:pointerWidth2;

    //背景缓存
    pointBufferSize= (pointerLength*2+widthTemp*2)*1.7*sizeof (llPoint);
    pPointBackup=(llPoint*)llMalloc(pointBufferSize);

    pPointColorBackup1=(llPointColor*)llMalloc(widthTemp*pointerLength*1.7*sizeof (llPointColor));
    pPointColorBackup2=(llPointColor*)llMalloc(widthTemp*pointerLength*1.7*sizeof (llPointColor));


        if((pNewWidget!=NULL)&&(pPointBackup!=NULL)&&(pPointColorBackup1!=NULL)&&(pPointColorBackup2!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->widgetType=widgetTypeGauge;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=10;
            pNewWidget->geometry.height=20;

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);
            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->deleteFunc=nGaugeDelete;
            pNewWidget->actionFunc=llGaugeAction;
            pNewWidget->refreshFunc=nGaugeRefresh;

            pNewWidget->imageAddr=imageAddr;
            pNewWidget->isColor=isColor;
            pNewWidget->backgroundColor=backgroundColor;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;

            pNewWidget->rotaryAngle=0;
            pNewWidget->angleBuffer=0;
            pNewWidget->originalPoint.x=originalPointX;
            pNewWidget->originalPoint.y=originalPointY;
            pNewWidget->originalPointerDistance=originalPointerDistance;
            pNewWidget->pointerLength=pointerLength;
            pNewWidget->pointerWidth1=pointerWidth1;
            pNewWidget->pointerWidth2=pointerWidth2;
            pNewWidget->pointerColor=pointerColor;

            pNewWidget->pointBackup=pPointBackup;
            pNewWidget->pointCount=0;
            pNewWidget->pointColorBuffer1=pPointColorBackup1;
            pNewWidget->pointColorCount1=0;
            pNewWidget->pointColorBuffer2=pPointColorBackup2;
            pNewWidget->pointColorCount2=0;
            pNewWidget->isBuffer1First=true;

            memset(pNewWidget->pointBackup,0,pointBufferSize);

            pGaugeLoad(pNewWidget,imageAddr);

            pNewWidget->loopCounter=0;

//            pGaugeRefresh(pNewWidget);
        }
        else
        {
            llFree(pPointBackup);
            llFree(pPointColorBackup1);
            llFree(pPointColorBackup2);
            llFree(pPointBackup);
        }
    }
    return pNewWidget;
}


//获取两点之间画直线的所有坐标
//返回坐标数量
static uint32_t llGaugeBresenhamLine( int16_t x0, int16_t y0, int16_t x1,int16_t y1, llPoint * pos )
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
        pos[curpixel].x=x;
        pos[curpixel].y=y;

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
    return curpixel;
}

bool llGaugeSamePointColor(int16_t x,int16_t y,llPointColor *source,uint32_t sourceLength,llColor *sourceRetColor)
{
    uint32_t i;

    for(i=0; i<sourceLength;i++)
    {
        if((source[i].x==x)&&((source[i].y==y)))
        {
            *sourceRetColor=source[i].color;
            return true;
        }
    }
    return false;
}

void llGaugeFillPolygon(llGauge *widget)
{
    int16_t i,line_length;
    uint32_t count,backup_count_temp=0;
    llColor targetColor,backupColor1,backupColor2;
    int16_t max_x,min_x;
    llPointColor *pOldBuf;
    uint32_t *pOldCount;
    llPointColor *pNewBuf;
    uint32_t *pNewCount;

    if(widget->isBuffer1First)
    {
        pNewBuf=widget->pointColorBuffer1;
        pNewCount=&widget->pointColorCount1;
        pOldBuf=widget->pointColorBuffer2;
        pOldCount=&widget->pointColorCount2;
    }
    else
    {
        pNewBuf=widget->pointColorBuffer2;
        pNewCount=&widget->pointColorCount2;
        pOldBuf=widget->pointColorBuffer1;
        pOldCount=&widget->pointColorCount1;
    }

    for(count=0; count<widget->pointCount;)
    {
        if(widget->pointBackup[count].y==widget->pointBackup[count+1].y)
        {
            //备份
            max_x=MAX(widget->pointBackup[count+1].x,widget->pointBackup[count].x);
            min_x=MIN(widget->pointBackup[count+1].x,widget->pointBackup[count].x);

            line_length=max_x-min_x+1;

            for(i=0; i<line_length; i++)
            {
                pNewBuf[backup_count_temp].x=min_x+i;
                pNewBuf[backup_count_temp].y=widget->pointBackup[count].y;
                if(!widget->isColor)
                {
                    if(llGaugeSamePointColor(pNewBuf[backup_count_temp].x,pNewBuf[backup_count_temp].y,pOldBuf,*pOldCount,&backupColor1)!=true)
                    {
                        if(llGaugeSamePointColor(pNewBuf[backup_count_temp].x,pNewBuf[backup_count_temp].y,pNewBuf,backup_count_temp,&backupColor2)!=true)
                        {
                            targetColor=llGetPoint(pNewBuf[backup_count_temp].x,pNewBuf[backup_count_temp].y);
                        }
                        else
                        {
                            targetColor=backupColor2;
                        }
                    }
                    else
                    {
                        targetColor=backupColor1;
                    }
                    pNewBuf[backup_count_temp].color=targetColor;
                }
                else
                {
                    pNewBuf[backup_count_temp].color=widget->backgroundColor;
                }

                backup_count_temp++;
            }

            //横向划线
            llDrawLine(widget->pointBackup[count].x,widget->pointBackup[count].y,widget->pointBackup[count+1].x,widget->pointBackup[count+1].y,widget->pointerColor);
             count=count+2;
        }
        else
        {
            pNewBuf[backup_count_temp].x=widget->pointBackup[count].x;
            pNewBuf[backup_count_temp].y=widget->pointBackup[count].y;
            if(!widget->isColor)
            {
                if(llGaugeSamePointColor(pNewBuf[backup_count_temp].x,pNewBuf[backup_count_temp].y,pOldBuf,*pOldCount,&backupColor1)!=true)
                {
                    targetColor=llGetPoint(pNewBuf[backup_count_temp].x,pNewBuf[backup_count_temp].y);
                }
                else
                {
                    targetColor=backupColor1;
                }
                pNewBuf[backup_count_temp].color=targetColor;
            }
            else
            {
                pNewBuf[backup_count_temp].color=widget->backgroundColor;
            }
            backup_count_temp++;
            llSetPoint(widget->pointBackup[count].x,widget->pointBackup[count].y,widget->pointerColor);
            count++;
        }
    }
    //返回数量
    *pNewCount=backup_count_temp;
}

void pGaugeRecovery(llGauge *widget)
{
    int32_t i,j;
    int16_t readX,readY;
    llColor readColor;
    bool isRecover;
    llPointColor *pOldBuf;
    uint32_t pOldCount;
    llPointColor *pNewBuf;
    uint32_t pNewCount;

    if(!widget->isBuffer1First)
    {
        pOldBuf=widget->pointColorBuffer1;
        pOldCount=widget->pointColorCount1;
        pNewBuf=widget->pointColorBuffer2;
        pNewCount=widget->pointColorCount2;
        widget->isBuffer1First=true;
    }
    else
    {
        pOldBuf=widget->pointColorBuffer2;
        pOldCount=widget->pointColorCount2;
        pNewBuf=widget->pointColorBuffer1;
        pNewCount=widget->pointColorCount1;
        widget->isBuffer1First=false;
    }

    readColor=widget->backgroundColor;
    //必须逆向恢复
    for(i=pOldCount-1; i>=0; i--)
    {
        readX=pOldBuf[i].x;
        readY=pOldBuf[i].y;

        //判断是否需要恢复背景

        isRecover=true;
        for(j=0; j<pNewCount; j++)
        {
            if((pNewBuf[j].x==readX)&&(pNewBuf[j].y==readY))
            {
                isRecover=false;
                break;
            }
        }

        if(isRecover==true)
        {
            if(!widget->isColor)
            {
                readColor=pOldBuf[i].color;
            }
            llSetPoint(readX,readY,readColor);
        }

    }
}

#define SWAP(a,b) (a^=b,b^=a,a^=b)

void llGaugeBubbleSort(llPoint *buffer,uint32_t length)
{
    uint32_t i,j,k;

    uint32_t count=0,startPos=0,startValue=0;
    //冒泡排序,排坐标Y
    for(i=0; i<length-1; i++) //n个数的数列总共扫描n-1次
    {
        for(j=0; j<length-i-1; j++) //每一趟扫描到a[n-i-2]与a[n-i-1]比较为止结束
        {
            if(buffer[j].y>buffer[j+1].y)//后一位数比前一位数小的话，就交换两个数的位置（升序）
            {
                SWAP(buffer[j].x,buffer[j+1].x);
                SWAP(buffer[j].y,buffer[j+1].y);
            }
        }
    }

    for(k=0; k<length-1; k++) //n个数的数列总共扫描n-1次
    {
        if(buffer[k].y==buffer[k+1].y)
        {
            count++;
            if(startValue!=buffer[k].y)
            {
                startValue=buffer[k].y;
                startPos=k;
            }
        }
        else
        {
            if(count>0)
            {
                count++;

                //冒泡排序,排坐标X
                for(i=0; i<count-1; i++) //n个数的数列总共扫描n-1次
                {
                    for(j=0; j<count-i-1; j++) //每一趟扫描到a[n-i-2]与a[n-i-1]比较为止结束
                    {
                        if(buffer[startPos+j].x>buffer[startPos+j+1].x)//后一位数比前一位数小的话，就交换两个数的位置（升序）
                        {
                            SWAP(buffer[startPos+j].x,buffer[startPos+j+1].x);
                        }
                    }
                }
                count=0;
            }
        }
    }

    if(count>0)
    {
        count++;

        //冒泡排序,排坐标X
        for(i=0; i<count-1; i++) //n个数的数列总共扫描n-1次
        {
            for(j=0; j<count-i-1; j++) //每一趟扫描到a[n-i-2]与a[n-i-1]比较为止结束
            {
                if(buffer[startPos+j].x>buffer[startPos+j+1].x)//后一位数比前一位数小的话，就交换两个数的位置（升序）
                {
                    SWAP(buffer[startPos+j].x,buffer[startPos+j+1].x);
                }
            }
        }
        count=0;
    }
}

void llGaugeDeleteArr(llPoint *buffer, uint32_t pos, uint32_t length)//M是位置，val是被删除的值，n是数组个数
{
    for(uint32_t i = pos; i < length; i++)
    {
        buffer[i] = buffer[i+1];
    }
    buffer[length-1].x=0;
    buffer[length-1].y=0;
}

uint32_t llGaugeAutoDeleteSameArrX(llPoint *buffer,uint32_t length)
{
    uint32_t k;

    for(k=0; k<(length-2);) //n个数的数列总共扫描n-1次
    {
        if((buffer[k].y==buffer[k+1].y)&&((buffer[k].y==buffer[k+2].y)))
        {
            llGaugeDeleteArr(buffer,k+1,length);
            length--;
        }
        else
        {
            k++;
        }
    }
    return k+2;
}

uint32_t llGaugeAutoDeleteSameArr(llPoint *buffer,uint32_t length)
{
    uint32_t k;

    for(k=0; k<length;)
    {
        if((buffer[k].y==buffer[k+1].y)&&((buffer[k].x==buffer[k+1].x)))
        {
            llGaugeDeleteArr(buffer,k+1,length);
            length--;
        }
        else
        {
            k++;
        }
    }
    return k;
}

void pGaugePointerRotary(llGauge *widget)
{
//    图片上任意点(x,y)，绕一个坐标点(rx0,ry0)逆时针旋转RotaryAngle角度
//    新的坐标设为(x', y')，公
//    x'= (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
//    y'=-(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;

//    求源坐标点的公式
//    x=(x'- rx0)*cos(RotaryAngle) - (y'- ry0)*sin(RotaryAngle) + rx0 ;
//    y=(x'- rx0)*sin(RotaryAngle) + (y'- ry0)*cos(RotaryAngle) + ry0 ;

    int16_t x0,y0;//旋转中心
    llPoint drawEndpoint[4];//4个端点
    float sina,cosa;
    uint8_t u8Temp;
    int16_t xPosTemp,yPosTemp;
//    int rotaryAngle;
    llPoint posOffset;

    //获取偏移坐标
    posOffset=llListGetGlobalPos((llGeneral*)widget);

    //整理角度值,大于360度,转换为0-359度
    widget->rotaryAngle%=360;

    sina=sina_val[360-widget->rotaryAngle]; //361个角
    cosa=cosa_val[360-widget->rotaryAngle]; //361个角

    //计算原点真实坐标
    if((widget->originalPoint.x==0)&&(widget->originalPoint.y==0))//没有设定原点就默认图片中间
    {
        x0=widget->geometry.width/2+posOffset.x;
        y0=widget->geometry.height/2+posOffset.y;
    }
    else
    {
        x0=widget->originalPoint.x+posOffset.x;
        y0=widget->originalPoint.y+posOffset.y;
    }

    //初始化端点偏移前的坐标
    //指针按照5:1
    drawEndpoint[0].x=x0-widget->pointerWidth1/2;
    drawEndpoint[0].y=y0+widget->originalPointerDistance;
    drawEndpoint[1].x=x0+widget->pointerWidth1/2;
    drawEndpoint[1].y=y0+widget->originalPointerDistance;
    drawEndpoint[2].x=x0+widget->pointerWidth2/2;
    drawEndpoint[2].y=y0+widget->originalPointerDistance+widget->pointerLength;
    drawEndpoint[3].x=x0-widget->pointerWidth2/2;
    drawEndpoint[3].y=y0+widget->originalPointerDistance+widget->pointerLength;

    //计算端点旋转后的坐标
    for(u8Temp=0; u8Temp<4; u8Temp++)
    {
//      x'= (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
//      y'=-(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;
        xPosTemp=(drawEndpoint[u8Temp].x-x0)*cosa +(drawEndpoint[u8Temp].y-y0)*sina+x0;
        yPosTemp=-(drawEndpoint[u8Temp].x-x0)*sina +(drawEndpoint[u8Temp].y-y0)*cosa+y0;

        drawEndpoint[u8Temp].x=xPosTemp;
        drawEndpoint[u8Temp].y=yPosTemp;
    }
    widget->pointCount=0;
    //获取外框坐标数据
    widget->pointCount=llGaugeBresenhamLine(drawEndpoint[0].x,drawEndpoint[0].y,drawEndpoint[1].x,drawEndpoint[1].y,&widget->pointBackup[widget->pointCount]);
    widget->pointCount+=llGaugeBresenhamLine(drawEndpoint[2].x,drawEndpoint[2].y,drawEndpoint[1].x,drawEndpoint[1].y,&widget->pointBackup[widget->pointCount]);
    widget->pointCount+=llGaugeBresenhamLine(drawEndpoint[2].x,drawEndpoint[2].y,drawEndpoint[3].x,drawEndpoint[3].y,&widget->pointBackup[widget->pointCount]);
    widget->pointCount+=llGaugeBresenhamLine(drawEndpoint[0].x,drawEndpoint[0].y,drawEndpoint[3].x,drawEndpoint[3].y,&widget->pointBackup[widget->pointCount]);

    //整理周长坐标
    llGaugeBubbleSort(widget->pointBackup,widget->pointCount);
    widget->pointCount=llGaugeAutoDeleteSameArrX(widget->pointBackup,widget->pointCount);
    widget->pointCount=llGaugeAutoDeleteSameArr(widget->pointBackup,widget->pointCount);

    //使用两个buf ，轮流保存新数据。另外一个buf保存旧数据；先显示新数据，在恢复旧背景

    llGaugeFillPolygon(widget);

    pGaugeRecovery(widget);

}

void pGaugeSetRotaryAngle(llGauge *widget,int16_t rotaryAngle,bool isInit)
{
    if(widget->isEnable)
    {
        widget->rotaryAngle=rotaryAngle;
        if(isInit)
        {
            llTimer_start(&widget->loopCounter);
            widget->angleBuffer=rotaryAngle;
            pGaugePointerRotary(widget);
        }
    }
}

void nGaugeSetRotaryAngle(uint16_t nameId,int16_t rotaryAngle,bool isInit)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGauge);
    if(widget!=NULL)
    {
        pGaugeSetRotaryAngle((llGauge*)widget,rotaryAngle,isInit);
    }
}

int16_t pGaugeGetRotaryAngle(llGauge *widget)
{
    return widget->angleBuffer;
}

int16_t nGaugeGetRotaryAngle(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGauge);
    if(widget!=NULL)
    {
        return pGaugeGetRotaryAngle((llGauge*)widget);
    }
    return 0;
}

void pGaugeLoop(llGauge *widget,uint16_t refreshMs,uint8_t moveValue)
{
    int16_t backup_target_angle;
    int16_t cal_angle;

    if(widget->isEnable)
    {
        if(llTimer_timeOut(&widget->loopCounter,refreshMs,true))
        {
            if(widget->rotaryAngle!=widget->angleBuffer)
            {
                backup_target_angle=widget->rotaryAngle;

                cal_angle=widget->rotaryAngle-widget->angleBuffer;

                if(moveValue==0)
                {
                    moveValue=1;
                }

                if(cal_angle>0)
                {
                    if(cal_angle>moveValue)
                    {
                        widget->angleBuffer=widget->angleBuffer+moveValue;
                    }
                    else
                    {
                        widget->angleBuffer++;
                    }
                }
                else
                {
                    if(cal_angle<moveValue)
                    {
                        widget->angleBuffer=widget->angleBuffer-moveValue;
                    }
                    else
                    {
                        widget->angleBuffer--;
                    }
                }
                widget->rotaryAngle=widget->angleBuffer;
                pGaugePointerRotary(widget);
                widget->rotaryAngle=backup_target_angle;
            }
        }
    }
}

void nGaugeLoop(uint16_t nameId,uint16_t refreshMs,uint8_t moveValue)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGauge);
    if(widget!=NULL)
    {
        pGaugeLoop((llGauge*)widget,refreshMs,moveValue);
    }
}


void pGaugeSetEnabled(llGauge *widget, bool state)
{
    widget->isEnable=state;
}

void nGaugeSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGauge);
    if(widget!=NULL)
    {
        pGaugeSetEnabled((llGauge*)widget,state);
    }
}


void pGaugeSetHidden(llGauge *widget,bool isHidden)
{
    if(widget->isEnable)
    {
        if(widget->isHidden)
        {
            if(isHidden==false)
            {
                //变为隐藏
                llGeneralWidgetParentRecover((llGeneral*)widget,widget->geometry);
            }
        }
        else
        {
            if(isHidden==true)
            {
                //变为显示
                pGaugeRefresh(widget);
            }
        }
        widget->isHidden=isHidden;
    }
}

void nGaugeSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGauge);
    if(widget!=NULL)
    {
        pGaugeSetHidden(widget,isHidden);
    }
}

