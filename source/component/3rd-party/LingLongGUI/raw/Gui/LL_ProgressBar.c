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

#include "LL_ProgressBar.h"
#include "LL_Linked_List.h"

void pProgressBarRefresh(llProgressBar *widget)
{
    int16_t x,y,width,height;
    uint16_t u16Temp;
    llPoint globalPos;


    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);

            x=widget->geometry.x+globalPos.x;
            y=widget->geometry.y+globalPos.y;
            width=widget->geometry.width-widget->textInfo.geometry.width;
            height=widget->geometry.height;

            if(widget->isMicroRefresh==false)
            {
                llFillSingleColor(x,y,x+width-1,y+height-1,widget->backgroundColor);
            }

            u16Temp=width*widget->percent/100.0;
            if(widget->percentWidth<u16Temp)//差值,刷新
            {
                llFillSingleColor(x+widget->percentWidth,y,x+u16Temp,y+height-1,widget->progressColor);
            }
            else
            {
                if(widget->percentWidth>u16Temp)//差值,刷新
                {
                    llFillSingleColor(x+u16Temp,y,x+widget->percentWidth,y+height-1,widget->backgroundColor);
                }
                else//相同值,刷新
                {
                    llFillSingleColor(x,y,x+widget->percentWidth,y+height-1,widget->progressColor);
                }
            }
            widget->percentWidth=u16Temp;

            //边框
            llFillSingleColor(x,y,x,y+height-1,RGB_CONVERT(188,188,188));
            llFillSingleColor(x,y,x+width-1,y,RGB_CONVERT(188,188,188));
            llFillSingleColor(x+width-1,y,x+width-1,y+height-1,RGB_CONVERT(188,188,188));
            llFillSingleColor(x,y+height-1,x+width-1,y+height-1,RGB_CONVERT(188,188,188));

            //显示百分比
            widget->textInfo.geometry.x=globalPos.x+widget->geometry.x+widget->geometry.width-widget->textInfo.fontLibInfo->fontSize*4;
            widget->textInfo.geometry.y=globalPos.y+widget->geometry.y;

            widget->textInfo.showGeometry=widget->textInfo.geometry;

            llGeneralWidgetParentRecover((llGeneral*)widget,widget->textInfo.geometry);

            llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t*)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);
            llCharDisplay(&(widget->textInfo));
        }
    }
}

void nProgressBarRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeProgressBar);
    if(widget!=NULL)
    {
        pProgressBarRefresh((llProgressBar*)widget);
    }
}

void pProgressBarFree(llProgressBar *widget)
{
    llFree(widget->textInfo.text);
    llFree(widget);
}

void nProgressBarDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llProgressBar *widget;

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
                pProgressBarFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llProgressBarAction(void *widget,uint8_t touchSignal)
{

}

llProgressBar *llProgressBarQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                        llFontLib *fontLib,llColor textColor,llColor bgColor,llColor progressColor,
                                        uint8_t percent,bool isHidden)
{
    llProgressBar * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;
    uint8_t *pText = NULL;

    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llProgressBar);
        pText=(uint8_t *)llMalloc(5*sizeof(uint8_t));

        if((pNewWidget!=NULL)&&(pText!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeProgressBar;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;
            pNewWidget->percent=percent;

            pNewWidget->backgroundColor=bgColor;
            pNewWidget->progressColor=progressColor;

            pNewWidget->isMicroRefresh=false;
            pNewWidget->percentWidth=0;

            //add linked list
            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);
            pNewWidget->deleteFunc=nProgressBarDelete;
            pNewWidget->actionFunc=llProgressBarAction;
            pNewWidget->refreshFunc=nProgressBarRefresh;

            pNewWidget->textInfo.backgroundColor=0;
            pNewWidget->textInfo.isTransparent=true;
            pNewWidget->textInfo.charColor=textColor;
            pNewWidget->textInfo.fontLibInfo=fontLib;
            pNewWidget->textInfo.geometry.x=x+width-fontLib->fontSize*4;
            pNewWidget->textInfo.geometry.y=y;
            pNewWidget->textInfo.geometry.width=fontLib->fontSize*4;
            pNewWidget->textInfo.geometry.height=height;
            pNewWidget->textInfo.hAlign=llAlignHCenter;
            pNewWidget->textInfo.vAlign=llAlignVCenter;
            pNewWidget->textInfo.text=pText;
            pNewWidget->textInfo.isAutoLineBreak=false;
            pNewWidget->textInfo.isPassword=false;

            llI32ToStr(pNewWidget->percent,pText);
            strcat((char*)pText,"%");

            pProgressBarRefresh(pNewWidget);
        }
        else
        {
            llFree(pText);
            llFree(pNewWidget);
        }
    }
    return pNewWidget;
}

llProgressBar *llProgressBarCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,llFontLib *fontLib,bool isHidden)
{
    return llProgressBarQuickCreate(nameId,parentNameId, x, y, width, height,
                                    fontLib, 0, RGB_CONVERT(230,230,230), RGB_CONVERT(6,176,37),
                                    0,isHidden);
}


uint8_t pProgressBarGetPercent(llProgressBar* widget)
{
    return widget->percent;
}

uint8_t nProgressBarGetPercent(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeProgressBar);
    if(widget!=NULL)
    {
        return pProgressBarGetPercent((llProgressBar*)widget);
    }
    return 0;
}

void pProgressBarSetPercent(llProgressBar* widget,uint8_t percent)
{
    if(widget->isEnable)
    {
        if(percent>100)
        {
            percent=100;
        }
        widget->percent=percent;
        widget->isMicroRefresh=true;
        memset(widget->textInfo.text,0,5);
        llI32ToStr(percent,widget->textInfo.text);
        strcat((char*)widget->textInfo.text,"%");
//        sprintf((char*)widget->textInfo.text, "%d%%", percent);
        pProgressBarRefresh(widget);
        widget->isMicroRefresh=false;
    }
}
void nProgressBarSetPercent(uint16_t nameId,uint8_t percent)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeProgressBar);
    if(widget!=NULL)
    {
        pProgressBarSetPercent((llProgressBar*)widget,percent);
    }
}


void pProgressBarSetEnabled(llProgressBar *widget, bool state)
{
    widget->isEnable=state;
}

void nProgressBarSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeProgressBar);
    if(widget!=NULL)
    {
        pProgressBarSetEnabled((llProgressBar*)widget,state);
    }
}


void pProgressBarSetHidden(llProgressBar *widget,bool isHidden)
{
    if(widget->isEnable)
    {
        if(widget->isHidden)
        {
            if(isHidden==false)
            {
                //��Ϊ����
                llGeneralWidgetParentRecover((llGeneral*)widget,widget->geometry);
            }
        }
        else
        {
            if(isHidden==true)
            {
                //��Ϊ��ʾ
                pProgressBarRefresh(widget);
            }
        }
        widget->isHidden=isHidden;
    }
}

void nProgressBarSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeProgressBar);
    if(widget!=NULL)
    {
        pProgressBarSetHidden(widget,isHidden);
    }
}


