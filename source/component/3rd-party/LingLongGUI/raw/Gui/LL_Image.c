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

#include "LL_Image.h"
#include "LL_Linked_List.h"
#include "stdio.h"
#include "LL_Config.h"
void llImageAction(void *widget, uint8_t touchSignal);

void nImageDelete(uint16_t nameId);



//自动根据图片大小更新尺寸数据
llImage *llImageCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x,uint16_t y,uint32_t imageAddr,bool isHidden)
{
    llImage * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llImage);
        if(pNewWidget!=NULL)
        {
            pNewWidget->nameId=nameId;

            pNewWidget->widgetType=widgetTypeImage;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=10;
            pNewWidget->geometry.height=20;

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);
            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->deleteFunc=nImageDelete;
            pNewWidget->actionFunc=llImageAction;
            pNewWidget->refreshFunc=nImageRefresh;

            pNewWidget->imageAddr=imageAddr;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;

            pImageRefresh(pNewWidget);
        }
        else
        {
            llFree(pNewWidget);
        }
    }
    return pNewWidget;
}

void llImageAction(void *widget,uint8_t touchSignal)
{
    if(((llImage*)widget)->isEnable)
    {
        if((touchSignal==SIGNAL_CLICK_PRESSED)||(touchSignal==SIGNAL_CLICK_RELEASED))
        {
            llEmitSignal(widget,touchSignal);
        }
    }
}

void pImageFree(llImage *widget)
{
    llFree(widget);
}

void nImageDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llImage *widget;
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
                pImageFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

bool pImageLoad(llImage *widget, uint32_t imageAddr)
{
    llGeometry newGeometry;
    llPoint globalPos;
    if(widget->isEnable)
    {
        globalPos=llListGetGlobalPos(widget->parentWidget);

        widget->imageAddr=imageAddr;

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

bool nImageLoad(uint16_t nameId, uint32_t imageAddr)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeImage);
    if(widget!=NULL)
    {
        return pImageLoad((llImage*)widget,imageAddr);
    }
    return false;
}

void pImageRefresh(llImage *widget)
{
    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
        pImageLoad(widget,widget->imageAddr);
        }
    }
}

void nImageRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeImage);
    if(widget!=NULL)
    {
        pImageRefresh((llImage*)widget);
    }
}

void pImageSetEnabled(llImage *widget, bool state)
{
    widget->isEnable=state;
}

void nImageSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeImage);
    if(widget!=NULL)
    {
        pImageSetEnabled((llImage*)widget,state);
    }
}

void pImageSetHidden(llImage *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pImageRefresh(widget);
    }
}

void nImageSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeImage);
    if(widget!=NULL)
    {
        pImageSetHidden(widget,isHidden);
    }
}

void pImageMove(llImage *widget,int16_t x, int16_t y)
{
    llGeometry oldGeometry,newGeometry;
    oldGeometry=widget->geometry;
    widget->isHidden=false;
    widget->geometry.x=x;
    widget->geometry.y=y;
    newGeometry=widget->geometry;
    llGeneralWidgetParentRecoverMove((llGeneral*)widget,oldGeometry,newGeometry);
    pImageRefresh(widget);
}

void nImageMove(uint16_t nameId,int16_t x, int16_t y)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeImage);
    if(widget!=NULL)
    {
        pImageMove(widget,x,y);
    }
}
