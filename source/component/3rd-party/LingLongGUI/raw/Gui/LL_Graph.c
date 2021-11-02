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

#include "LL_Graph.h"
#include "LL_Linked_List.h"
#include "LL_Background.h"
#include "LL_User.h"


void pGraphFree(llGraph *widget)
{
    llFree(widget->pointBuff);
    llFree(widget);
}

void nGraphDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llGraph *widget;

    if(llList_GetInfoByName(&linkInfo,nameId)==true)
    {
        widget=linkInfo->widget;

        //查找父链表
        llList_GetInfoByName(&parentInfo,((llGeneral*)widget->parentWidget)->nameId);
        //消除自身在父链表中的位置数据
        list_for_each_prev_safe(tempPos, safePos,&parentInfo->child_link)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
            if(tempInfo->widget==widget)
            {
                llLinkedListDelete(tempPos);
                pGraphFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llGraphAction(void *widget,uint8_t touchSignal)
{

}

void pGraphRefresh(llGraph *widget)
{
    llPoint globalPos,graphOriginPos;
    uint8_t vLineCount=0,hLineCount=0;//垂直，水平
    uint8_t i;
    
    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
    globalPos=llListGetGlobalPos(widget->parentWidget);
    graphOriginPos.x=widget->geometry.x+globalPos.x;
    graphOriginPos.y=widget->geometry.y+globalPos.y+widget->geometry.height-1;

    //画背景
    llFillSingleColor(widget->geometry.x+globalPos.x,widget->geometry.y+globalPos.y,widget->geometry.x+globalPos.x+widget->geometry.width-1,widget->geometry.y+globalPos.y+widget->geometry.height-1,widget->backgroundColor);


    //画网格
    //计算画多少条线
    vLineCount=widget->geometry.height/widget->verticalInterval;
    hLineCount=widget->geometry.width/widget->horizontalInterval;

    if(widget->isHideGrid==false)
    {
        for (i=0;i<vLineCount;i++)
        {
            llDrawLine(graphOriginPos.x,graphOriginPos.y-(widget->verticalInterval*(i)),graphOriginPos.x+widget->geometry.width-1,graphOriginPos.y-(widget->verticalInterval*(i)),widget->gridColor);
        }

        for (i=0;i<hLineCount;i++)
        {
            llDrawLine(graphOriginPos.x+(widget->horizontalInterval*i),graphOriginPos.y,graphOriginPos.x+(widget->horizontalInterval*i),graphOriginPos.y-(widget->geometry.height-1),widget->gridColor);
        }
    }
    else
    {
        llDrawLine(graphOriginPos.x,graphOriginPos.y,graphOriginPos.x+widget->geometry.width-1,graphOriginPos.y,widget->gridColor);
        llDrawLine(graphOriginPos.x,graphOriginPos.y,graphOriginPos.x,graphOriginPos.y-(widget->geometry.height-1),widget->gridColor);
    }

    //画点
    for(i=0;i<widget->pointSize;i++)
    {
        llSetPoint(graphOriginPos.x+widget->pointBuff[i].x,graphOriginPos.y-widget->pointBuff[i].y,RGB_CONVERT(255,255,255));
    }
}
}
}


void nGraphRefresh(uint16_t nameId)
{
    llListWidgetInfo *widgetInfo;

    if(llList_GetInfoByName(&widgetInfo,nameId)==true)
    {
        if(((llGeneral*)widgetInfo->widget)->widgetType==widgetTypeGraph)
        {
            pGraphRefresh((llGraph*)(widgetInfo->widget));
        }
    }
}

llGraph *llGraphCreate(uint16_t nameId, uint16_t parentNameId , int16_t x, int16_t y , int16_t width, int16_t height,uint16_t pointSize,bool isHidden)
{
    llGraph * pNewWidget = NULL;
    llPoint * pNewPointBuff;
    llListWidgetInfo *parentInfo;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
    pNewWidget = LL_MALLOC_WIDGET_INFO(llGraph);
    pNewPointBuff = llMalloc(sizeof (llPoint)*pointSize);

        if((pNewWidget!=NULL)&&(pNewPointBuff!=NULL))
        {
            pNewWidget->nameId=nameId;
            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeGraph;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->isHidden=isHidden;

            pNewWidget->backgroundColor=RGB_CONVERT(0,0,0);
            pNewWidget->gridColor=RGB_CONVERT(255,255,255);
            pNewWidget->isHideGrid=true;
            pNewWidget->verticalInterval=20;
            pNewWidget->horizontalInterval=20;

            pNewWidget->pointSize=pointSize;
            pNewWidget->pointBuff=pNewPointBuff;
            pNewWidget->pointCount=0;

            //add linked list

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            pNewWidget->deleteFunc=nGraphDelete;
            pNewWidget->actionFunc=llGraphAction;
            pNewWidget->refreshFunc=nGraphRefresh;


            pGraphRefresh(pNewWidget);

        }
        else
        {
            llFree(pNewWidget);
        }
    }


    return pNewWidget;
}

void pGraphAddPoint(llGraph *widget,int16_t x,int16_t y)
{
    llPoint pos;
    pos.x=x;
    pos.y=y;
    if(widget->pointCount<widget->pointSize)
    {
        widget->pointBuff[widget->pointCount]=pos;
        widget->pointCount++;
    }
    else
    {
        widget->pointCount=0;
        widget->pointBuff[widget->pointCount]=pos;
    }
}

llGraph* llGraphGetWidget(uint16_t nameId)
{
    llListWidgetInfo *widgetInfo;

    if(llList_GetInfoByName(&widgetInfo,nameId)==true)
    {
        if(((llGeneral*)widgetInfo->widget)->widgetType==widgetTypeGraph)
        {
            return (llGraph*)widgetInfo->widget;
        }
    }
    return 0;
}

void pGraphSetHidden(llGraph *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pGraphRefresh(widget);
    }
}

void nGraphSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeGraph);
    if(widget!=NULL)
    {
        pGraphSetHidden(widget,isHidden);
    }
}
