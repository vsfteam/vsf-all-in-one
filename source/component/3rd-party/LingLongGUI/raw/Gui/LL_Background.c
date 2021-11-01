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

#include "LL_Background.h"
#include "LL_Linked_List.h"

void llBackgroundAction(void *widget,uint8_t touchSignal);

llBackground *llBackgroundQuickCreate(uint16_t nameId,int16_t width,int16_t height,bool isColor,llColor color,uint32_t imageAddr)
{
    llBackground * pNewWidget;

    pNewWidget = LL_MALLOC_WIDGET_INFO(llBackground);

    if(pNewWidget!=NULL)
    {
        pNewWidget->nameId=nameId;
        pNewWidget->widgetType=widgetTypeBackground;

        pNewWidget->geometry.x=0;
        pNewWidget->geometry.y=0;
        pNewWidget->geometry.width=width;
        pNewWidget->geometry.height=height;

        pNewWidget->deleteFunc=nBackgroundDelete;
        pNewWidget->actionFunc=llBackgroundAction;
        pNewWidget->isHidden=false;
        pNewWidget->parentWidget=NULL;
        pNewWidget->parentType=widgetTypeNone;

        cfgMonitorWidth=pNewWidget->geometry.width;
        cfgMonitorHeight=pNewWidget->geometry.height;
        pNewWidget->isColor=isColor;
        pNewWidget->color=color;
        pNewWidget->imageAddr=imageAddr;
        
        if(isColor)
        {
            llFillSingleColor(pNewWidget->geometry.x,
                              pNewWidget->geometry.y,
                              pNewWidget->geometry.x+pNewWidget->geometry.width-1,
                              pNewWidget->geometry.y+pNewWidget->geometry.height-1,
                              pNewWidget->color);
        }
        else
        {
            llDoubleBufferStart();
            llGeneralImageShow((llGeneral*)pNewWidget,pNewWidget->imageAddr,pNewWidget->geometry.x,pNewWidget->geometry.y);
            llDoubleBufferEnd(true);
        }
        //add linked list
        llListWidgetAdd(&llWidgetLink,pNewWidget);

    }
    else
    {
        llFree(pNewWidget);
    }

    return pNewWidget;
}

llBackground *llBackgroundCreate(uint16_t nameId)
{
    return llBackgroundQuickCreate(nameId,CONFIG_MONITOR_WIDTH,CONFIG_MONITOR_HEIGHT,true,RGB_CONVERT(240,240,240),0);
}

void llBackgroundAction(void *widget,uint8_t touchSignal)
{

}

void pBackgroundFree(llBackground *widget)
{
    //    llFree(widget->name);
    llFree(widget);
}

void nBackgroundDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    //    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llListConnectInfo *linkConnectTempInfo;
    llBackground *widget;

    if(llList_GetInfoByName(&linkInfo,nameId)==true)
    {
        widget=linkInfo->widget;

        //清除子控件的数据
        list_for_each_prev_safe(tempPos, safePos, &linkInfo->child_link)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
            ((llGeneral *)tempInfo->widget)->deleteFunc(((llGeneral *)tempInfo->widget)->nameId);
        }

        //消除自身在根控件链表中的位置数据
        list_for_each_prev_safe(tempPos, safePos, &llWidgetLink)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);

            if(tempInfo->widget==widget)
            {
                llLinkedListDelete(tempPos);
                pBackgroundFree(widget);
                llFree(tempInfo);
            }
        }

        //清除connect链表,理论上应该要先核对connect链表上的sender，receiver和背景上的控件对应，才进行删除
        //实际上，背景就只最高等级的，所以直接清除所有connect数据
        list_for_each_prev_safe(tempPos, safePos, &llConnectLink)
        {
            linkConnectTempInfo = list_entry(tempPos, llListConnectInfo, parent_link_pos);
            llLinkedListDelete(tempPos);
            llFree(linkConnectTempInfo);
        }

        //清除信号队列,避免刷新背景后，误触发
        llQueueClear(emitQueue);
    }
}



void pBackgroundSetColor(llBackground *widget,llColor color)
{
    widget->isColor=true;
    widget->color=color;
    llFillSingleColor(widget->geometry.x,
                      widget->geometry.y,
                      widget->geometry.x+widget->geometry.width-1,
                      widget->geometry.y+widget->geometry.height-1,
                      widget->color);
}

void nBackgroundSetColor(uint16_t nameId,llColor color)
{
    llListWidgetInfo *info;
    if(llList_GetInfoByName(&info,nameId)==true)
    {
        if(((llGeneral*)(info->widget))->widgetType==widgetTypeBackground)
        {
            pBackgroundSetColor(info->widget,color);
        }
    }
}

void pBackgroundSetImage(llBackground *widget,uint32_t imageAddr)
{
    widget->isColor=false;
    widget->imageAddr=imageAddr;
    llDoubleBufferStart();  
    llGeneralImageShow((llGeneral*)widget,widget->imageAddr,widget->geometry.x,widget->geometry.y);
    llDoubleBufferEnd(true);
}

void nBackgroundSetImage(uint16_t nameId,uint32_t imageAddr)
{
    llListWidgetInfo *info;
    if(llList_GetInfoByName(&info,nameId)==true)
    {
        if(((llGeneral*)(info->widget))->widgetType==widgetTypeBackground)
        {
            pBackgroundSetImage(info->widget,imageAddr);
        }
    }
}


void pBackgroundRefresh(llBackground *widget)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *tempInfo;

    if(widget->isColor==true)
    {
        pBackgroundSetColor(widget,widget->color);
    }
    else
    {
        pBackgroundSetImage(widget,widget->imageAddr);
    }

    if(llList_GetInfoByName(&linkInfo,widget->nameId)==true)
    {
        //正向刷新子控件
        list_for_each_safe(tempPos, safePos, &linkInfo->child_link)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
            //不能变更定子控件隐藏参数
            //                ((llGeneral *)tempInfo->widget)->isHide=false;
            ((llGeneral *)tempInfo->widget)->refreshFunc(((llGeneral *)tempInfo->widget)->nameId);
        }
    }

}

void nBackgroundRefresh(uint16_t nameId)
{
    llListWidgetInfo *info;
    if(llList_GetInfoByName(&info,nameId)==true)
    {
        if(((llGeneral*)(info->widget))->widgetType==widgetTypeBackground)
        {
            pBackgroundRefresh(info->widget);
        }
    }
}

void pBackgroundMove(llBackground *widget,int16_t x,int16_t y)
{
    widget->geometry.x=x;
    widget->geometry.y=y;
    pBackgroundRefresh(widget);
}

void nBackgroundMove(uint16_t nameId, int16_t x, int16_t y)
{
    llListWidgetInfo *info;
    if(llList_GetInfoByName(&info,nameId)==true)
    {
        if(((llGeneral*)(info->widget))->widgetType==widgetTypeBackground)
        {
            pBackgroundMove(info->widget,x,y);
        }
    }
}

void pBackgroundSpecificAreaRecover(llBackground *widget,llGeometry geometry)
{
    if(widget->isColor)
    {
        llFillSingleColor(geometry.x,
                              geometry.y,
                              geometry.x+geometry.width-1,
                              geometry.y+geometry.height-1,
                              widget->color);
    }
    else
    {
        //局部刷图
        llGeneralImageSpecificAreaShow((llGeneral*)widget,widget->geometry,geometry,widget->imageAddr);
    }
}
