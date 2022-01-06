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

#include "LL_Window.h"
#include "LL_Linked_List.h"
#include "LL_Background.h"

void llWindowAction(void *widget,uint8_t touchSignal);
void pWindowSetHiden(llWindow *widget, bool state);
void nWindowRefresh(uint16_t nameId);
void pWindowRefresh(llWindow *widget);
void nWindowSetHiden(uint16_t nameId,bool state);
void nWindowDelete(uint16_t nameId);

llWindow *llWindowQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                llColor bgColor,
                                bool isTransparent,bool isHidden)
{
    llWindow * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llWindow);
        if(pNewWidget!=NULL)
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeWindow;
            pNewWidget->color=bgColor;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;
            pNewWidget->deleteFunc=nWindowDelete;
            pNewWidget->actionFunc=llWindowAction;
            pNewWidget->refreshFunc=nWindowRefresh;
            pNewWidget->isTransparent=isTransparent;
            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;
            pNewWidget->userInfo=NULL;
            
            //add linked list
            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            pWindowRefresh(pNewWidget);
        }
        else
        {
            llFree(pNewWidget);
        }
    }

    return pNewWidget;
}

llWindow *llWindowCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,bool isHidden)
{
    return llWindowQuickCreate(nameId,parentNameId,x,y,width,height,0,true,isHidden);
}

void pWindowSetColor(llWindow *widget,llColor color)
{
    llPoint globalPos;

    if(widget->isEnable)
    {
        globalPos=llListGetGlobalPos(widget->parentWidget);

        widget->isTransparent=false;
        widget->color=color;
        llFillSingleColor(widget->geometry.x+globalPos.x,
                          widget->geometry.y+globalPos.y,
                          widget->geometry.x+globalPos.x+widget->geometry.width-1,
                          widget->geometry.y+globalPos.y+widget->geometry.height-1,
                          widget->color);
    }
}

void nWindowSetColor(uint16_t nameId,llColor color)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeWindow);
    if(widget!=NULL)
    {
        pWindowSetColor(widget,color);
    }
}

void llWindowAction(void *widget,uint8_t touchSignal)
{

}

void pWindowSpecificAreaRecover(llWindow *widget,llGeometry geometry)
{
    if(widget->isTransparent)
    {
        llGeneralWidgetParentRecover((llGeneral*)widget,geometry);
    }
    else
    {
        llFillSingleColor(geometry.x,
                          geometry.y,
                          geometry.x+geometry.width-1,
                          geometry.y+geometry.height-1,
                          widget->color);
    }
}

void pWindowBackgroundRecover(llWindow *widget)
{
    //遍历控件,判断是否重叠
    llListHead *tempPos,*tempPos2,*safePos,*safePos2;
    llListWidgetInfo *tempInfo,*tempInfo2;
    llGeneral *tempWidget,*tempWidget2;
    llListWidgetInfo *linkInfo;

    //局部刷新背景图片或颜色

    if(((llGeneral*)widget->parentWidget)->widgetType==widgetTypeBackground)
    {
        pBackgroundSpecificAreaRecover((llBackground*)widget->parentWidget,widget->geometry);
    }
    else //窗体
    {
        //窗体重刷
        pWindowRefresh(widget);
    }

    //正向开始遍历
    list_for_each_safe(tempPos,safePos, &llWidgetLink)
    {
        tempInfo = list_entry(tempPos,llListWidgetInfo, parent_link_pos);

        tempWidget=(llGeneral*)tempInfo->widget;


        if(tempWidget==widget->parentWidget)//要同一个背景中,实际只有一个背景
        {
            //继续查子控件
            list_for_each_safe(tempPos2,safePos2, &(tempInfo->child_link))
            {
                tempInfo2 = list_entry(tempPos2, llListWidgetInfo, parent_link_pos);

                tempWidget2=(llGeneral*)tempInfo2->widget;

                if(tempWidget2!=(llGeneral*)widget)//排除窗体本身
                {
                    if(llRectIsOverlap(tempWidget2->geometry,widget->geometry)==true)
                    {
                        //遮盖的控件刷新
                        tempWidget2->refreshFunc(tempWidget2->nameId);
                    }
                }

            }
        }
    }

//    //设定窗体子控件隐藏参数
//    if(llList_GetInfoByName(&linkInfo,widget->nameId)==true)
//    {
//        //刷新子控件
//        list_for_each_prev_safe(tempPos, safePos, &linkInfo->child_link)
//        {
//            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
//            //设定子控件隐藏参数
//            ((llGeneral *)tempInfo->widget)->isHide=true;
//        }
//    }
}

void pWindowFree(llWindow *widget)
{
    llFree(widget->userInfo);
    //    llFree(widget->name);
    llFree(widget);
}

extern volatile bool isQuitPage;
void nWindowDelete(uint16_t nameId)
{
    llListHead *tempPos,*tempPos2,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *tempInfo;
    llWindow *widget;

    if(llList_GetInfoByName(&linkInfo,nameId)==true)
    {
        widget=linkInfo->widget;

        //清除子控件的数据
        list_for_each_prev_safe(tempPos, safePos, &linkInfo->child_link)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
            //清除connect链表
            llDeleteConnectSignal(((llGeneral *)tempInfo->widget)->nameId);
            ((llGeneral *)tempInfo->widget)->deleteFunc(((llGeneral *)tempInfo->widget)->nameId);
        }

        if(llListGetListByWidget(&tempPos2,widget)==true)
        {
            if(isQuitPage==false)
                        {
                            //先恢复后删除指针
                            pWindowBackgroundRecover(widget);//恢复遮挡背景和控件
                        }
                        llLinkedListDelete(tempPos2);
                        pWindowFree(widget);
                        llFree(tempPos2);
        }

        //清除connect链表
        llDeleteConnectSignal(nameId);

        //清除信号队列,避免刷新背景后，误触发
        llQueueClear(emitQueue);
    }
}


void pWindowRefresh(llWindow *widget)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *tempInfo;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
        //window颜色恢复
        if(widget->isTransparent==false)
        {
            pWindowSetColor(widget,widget->color);
        }

        if(llList_GetInfoByName(&linkInfo,widget->nameId)==true)
        {
            //刷新子控件
            list_for_each_prev_safe(tempPos, safePos, &linkInfo->child_link)
            {
                tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
                //必须先设定子控件隐藏参数
                ((llGeneral *)tempInfo->widget)->isHidden=false;
                ((llGeneral *)tempInfo->widget)->refreshFunc(((llGeneral *)tempInfo->widget)->nameId);
            }
        }
    }
    }
}

void nWindowRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeWindow);
    if(widget!=NULL)
    {
        pWindowRefresh((llWindow*)widget);
    }
}

void pWindowSetHiden(llWindow *widget,bool state)
{
    if(widget->isEnable)
    {
        if((state==true)&&(widget->isHidden==false))//窗体隐藏
        {
            widget->isHidden=true;
            //恢复遮挡背景和控件
            pWindowBackgroundRecover(widget);
        }
        else
        {
            if((state==false)&&(widget->isHidden==true))//窗体还原
            {
                //必须先设定控件隐藏参数
                widget->isHidden=false;
                //刷新window显示
                pWindowRefresh(widget);
            }
        }
    }
}

void nWindowSetHiden(uint16_t nameId,bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeWindow);
    if(widget!=NULL)
    {
        pWindowSetHiden((llWindow*)widget,state);
    }
}


void pWindowSetEnabled(llWindow *widget, bool state)
{
    widget->isEnable=state;
}

void nWindowSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeWindow);
    if(widget!=NULL)
    {
        pWindowSetEnabled((llWindow*)widget,state);
    }
}

