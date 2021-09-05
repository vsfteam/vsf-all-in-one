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

#include "LL_DateTime.h"
#include "LL_Linked_List.h"
#include "LL_Characters.h"
//#include "LL_LatticeLib.h"
#include "LL_User.h"
#include "LL_Window.h"
#include "LL_Background.h"

void pDateTimeRefresh(llDateTime *widget);

void pDateTimeFree(llDateTime *widget)
{
    llFree(widget->textInfo.text);
    llFree(widget->dateTimeFormat);
    llFree(widget);
}

void nDateTimeDelete(uint16_t nameId)
{
    llListHead *temp_pos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llDateTime *widget;

    if(llList_GetInfoByName(&linkInfo,nameId)==true)
    {
        widget=linkInfo->widget;

        //查找父链表
        llList_GetInfoByName(&parentInfo,((llGeneral*)widget->parentWidget)->nameId);
        //消除自身在父链表中的位置数据
        list_for_each_prev_safe(temp_pos,safePos, &parentInfo->child_link)
        {
            tempInfo = list_entry(temp_pos, llListWidgetInfo, parent_link_pos);
            if(tempInfo->widget==widget)
            {
                llLinkedListDelete(temp_pos);
                pDateTimeFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llDateTimeAction(void *widget,uint8_t touchSignal)
{
    bool clickState;
    llPoint pos;
    if(((llDateTime*)widget)->isEnable)
    {
        switch(touchSignal)
        {

        default:
            break;
        }
    }
}

void pDateTimeRefresh(llDateTime *widget)
{
    llPoint globalPos;
    llGeometry textGeometry;
    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);
            //更新文字坐标
            widget->textInfo.showGeometry.x=widget->geometry.x+globalPos.x;
            widget->textInfo.showGeometry.y=widget->geometry.y+globalPos.y;

            char tempBuf[5];

            uint16_t year;
            char *addr;
            uint8_t rtcBuf[8];
            // yy yy mm dd hh mm ss ww
            llGetRtc(rtcBuf);

            strcpy((char *)widget->textInfo.text,(const char *)widget->dateTimeFormat);
            addr=strstr((const char*)widget->textInfo.text,"yyyy");
            if(addr)
            {
                year=rtcBuf[0];
                year=year<<8;
                year=year+rtcBuf[1];
                snprintf(tempBuf,5,"%04d",year);
                memcpy(addr,tempBuf,4);
            }
            addr=strstr((const char*)widget->textInfo.text,"mm");
            if(addr)
            {
                snprintf(tempBuf,3,"%02d",rtcBuf[2]);
                memcpy(addr,tempBuf,2);
            }
            addr=strstr((const char*)widget->textInfo.text,"dd");
            if(addr)
            {
                snprintf(tempBuf,3,"%02d",rtcBuf[3]);
                memcpy(addr,tempBuf,2);
            }
            addr=strstr((const char*)widget->textInfo.text,"hh");
            if(addr)
            {
                snprintf(tempBuf,3,"%02d",rtcBuf[4]);
                memcpy(addr,tempBuf,2);
            }
            addr=strstr((const char*)widget->textInfo.text,"nn");
            if(addr)
            {
                snprintf(tempBuf,3,"%02d",rtcBuf[5]);
                memcpy(addr,tempBuf,2);
            }
            addr=strstr((const char*)widget->textInfo.text,"ss");
            if(addr)
            {
                snprintf(tempBuf,3,"%02d",rtcBuf[6]);
                memcpy(addr,tempBuf,2);
            }

            if(widget->textInfo.isTransparent)
            {
                llGeneralWidgetParentRecover((llGeneral*)widget,widget->textInfo.showGeometry);
            }
            
            llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t *)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);
            textGeometry=llCharDisplay(&(widget->textInfo));
        }
    }
}

void nDateTimeRefresh(uint16_t nameId)
{
    llListWidgetInfo *widgetInfo;

    if(llList_GetInfoByName(&widgetInfo,nameId)==true)
    {
        if(((llDateTime*)widgetInfo->widget)->widgetType==widgetTypeDateTime)
        {
            pDateTimeRefresh((llDateTime*)widgetInfo->widget);
        }
    }
}

bool slotDateTimeRefresh(llConnectInfo info)
{

    pDateTimeRefresh(info.receiver);

    return false;
}

llDateTime *llDateTimeQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                  uint8_t *dateTimeFormat,
                                  llFontLib *fontLib,llColor textColor,llColor bgColor,
                                  llHorizontalAlign hAlign,llVerticalAlign vAlign,bool isTransparent,bool isHidden)
{
    llDateTime * pNewWidget = NULL;
    uint8_t *pDateTimeFormat = NULL,*pText = NULL;
    llListWidgetInfo *parentInfo;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
    pNewWidget = LL_MALLOC_WIDGET_INFO(llDateTime);
    pText=LL_MALLOC_STRING(dateTimeFormat);
    pDateTimeFormat=LL_MALLOC_STRING(dateTimeFormat);
        if((pNewWidget!=NULL)&&(pText!=NULL)&&(pDateTimeFormat!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeDateTime;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;


            //add linked list

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            pNewWidget->deleteFunc=nDateTimeDelete;
            pNewWidget->actionFunc=llDateTimeAction;
            pNewWidget->refreshFunc=nDateTimeRefresh;
            pNewWidget->textInfo.backgroundColor=bgColor;
            pNewWidget->textInfo.isTransparent=isTransparent;
            pNewWidget->textInfo.charColor=textColor;
            pNewWidget->textInfo.fontLibInfo=fontLib;
            pNewWidget->textInfo.isAutoLineBreak=false;
            pNewWidget->textInfo.isPassword=false;

            pNewWidget->dateTimeFormat=pDateTimeFormat;
            strcpy((char *)pNewWidget->dateTimeFormat,(const char *)dateTimeFormat);

            pNewWidget->textInfo.geometry.x=x;
            pNewWidget->textInfo.geometry.y=y;
            pNewWidget->textInfo.geometry.width=width;
            pNewWidget->textInfo.geometry.height=height;
            pNewWidget->textInfo.showGeometry.width=width;
            pNewWidget->textInfo.showGeometry.height=height;
            pNewWidget->textInfo.hAlign=hAlign;
            pNewWidget->textInfo.vAlign=vAlign;
            pNewWidget->textInfo.text=pText;
            pNewWidget->textInfo.mallocLength=strlen((const char*)dateTimeFormat);

            strcpy((char *)pNewWidget->textInfo.text,(const char *)dateTimeFormat);

            pNewWidget->textInfo.charColor=textColor;

            pDateTimeRefresh(pNewWidget);


            llConnectSignal(((llBackground*)llListGetRootWidget())->nameId,SIGNAL_SYS_TIME_OUT,nameId,slotDateTimeRefresh);
        }
        else
        {
            llFree(pNewWidget);
            llFree(pText);
            llFree(pDateTimeFormat);
        }
    }

    return pNewWidget;
}

llDateTime *llDateTimeCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t *dateTimeFormat,llFontLib *fontLib,bool isHidden)
{
    return llDateTimeQuickCreate(nameId,parentNameId,x,y,width,height,dateTimeFormat,fontLib,0,RGB_CONVERT(255,255,255),llAlignHLeft,llAlignVCenter,false,isHidden);
}

void pDateTimeSetTransparent(llDateTime *widget,bool isTransparent)
{
    if(widget->isEnable)
    {
        widget->textInfo.isTransparent=isTransparent;
        pDateTimeRefresh(widget);
    }
}

void nDateTimeSetTransparent(uint16_t nameId,bool isTransparent)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetTransparent((llDateTime*)widget,isTransparent);
    }
}

void pDateTimeSetFont(llDateTime *widget,llFontLib *fontLib)
{
    if(widget->isEnable)
    {
        widget->textInfo.fontLibInfo=fontLib;
        pDateTimeRefresh(widget);
    }
}

void nDateTimeSetFont(uint16_t nameId,llFontLib *fontLib)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetFont((llDateTime*)widget,fontLib);
    }
}

void pDateTimeSetTextColor(llDateTime *widget,llColor color)
{
    if(widget->isEnable)
    {
        widget->textInfo.charColor=color;
        pDateTimeRefresh(widget);
    }
}

void nDateTimeSetTextColor(uint16_t nameId,llColor color)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetTextColor((llDateTime*)widget,color);
    }
}

void pDateTimeSetBackgroundColor(llDateTime *widget,llColor color)
{
    if(widget->isEnable)
    {
        widget->textInfo.isTransparent=false;
        widget->textInfo.backgroundColor=color;
        pDateTimeRefresh(widget);
    }
}

void nDateTimeSetBackgroundColor(uint16_t nameId,llColor color)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetBackgroundColor((llDateTime*)widget,color);
    }
}

void pDateTimeRecoverBackground(llDateTime *widget,llGeometry geometry)
{
    if(widget->textInfo.isTransparent)
    {
        llGeneralWidgetParentRecover((llGeneral*)widget,geometry);
//        if(widget->parentType==widgetTypeWindow)
//        {
//            pWindowSpecificAreaRecover(widget->parentWidget,geometry);
//        }
//        else
//        {
//            if(widget->parentType==widgetTypeBackground)
//            {
//                pBackgroundSpecificAreaRecover(widget->parentWidget,geometry);
//            }
//        }
    }
    else
    {
        llFillSingleColor(geometry.x,geometry.y,geometry.x+geometry.width,geometry.y+geometry.height,widget->textInfo.backgroundColor);
    }
}

void pDateTimeSetAlign(llDateTime *widget,llHorizontalAlign hAlign,llVerticalAlign vAlign)
{
    if(widget->isEnable)
    {
        widget->textInfo.hAlign=hAlign;
        widget->textInfo.vAlign=vAlign;
        pDateTimeRefresh(widget);
    }
}

void nDateTimeSetAlign(uint16_t nameId,llHorizontalAlign hAlign,llVerticalAlign vAlign)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetAlign((llDateTime*)widget,hAlign,vAlign);
    }
}


void pDateTimeSetEnabled(llDateTime *widget, bool state)
{
    widget->isEnable=state;
}

void nDateTimeSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetEnabled((llDateTime*)widget,state);
    }
}

void pDateTimeSetHidden(llDateTime *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pDateTimeRefresh(widget);
    }
}

void nDateTimeSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeDateTime);
    if(widget!=NULL)
    {
        pDateTimeSetHidden(widget,isHidden);
    }
}
