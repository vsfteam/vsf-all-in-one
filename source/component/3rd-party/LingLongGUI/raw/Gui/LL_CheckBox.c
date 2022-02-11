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

#include "LL_CheckBox.h"
#include "LL_Linked_List.h"

void pCheckBoxFree(llCheckBox *widget)
{
    llFree(widget->textInfo.text);
    llFree(widget->checkImagePath);
    llFree(widget->noCheckImagePath);
    llFree(widget);
}

void nCheckBoxDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llCheckBox *widget;

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
                pCheckBoxFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llCheckBoxAction(void *widget,uint8_t touchSignal)
{
    if(((llCheckBox*)widget)->isEnable)
    {
        if((touchSignal==SIGNAL_CLICK_PRESSED)||(touchSignal==SIGNAL_CLICK_RELEASED))
        {
            llEmitSignal(widget,touchSignal);
            llEmitSignal(widget,SIGNAL_VALUE_CHANGED);
        }
    }
}

void pCheckBoxShow(llCheckBox *widget)
{
    int16_t x,y,width,height;
    llPoint globalPos;
    globalPos=llListGetGlobalPos(widget->parentWidget);

    x=widget->geometry.x+globalPos.x;
    y=widget->geometry.y+globalPos.y;
    width=widget->geometry.width;
    height=widget->geometry.height;

    y+=(height/2)-6;
    
    if(widget->isChecked==true)
    {
        if(widget->checkImagePath!=NULL)
        {

        }
        else
        {
            //画方框，打钩
            llFillSingleColor(x,y,x+12,y+12,RGB_CONVERT(244,244,244));
            llDrawLine(x,y,x+12,y,RGB_CONVERT(142,143,143));
            llDrawLine(x,y,x,y+12,RGB_CONVERT(142,143,143));
            llDrawLine(x+12,y,x+12,y+12,RGB_CONVERT(142,143,143));
            llDrawLine(x,y+12,x+12,y+12,RGB_CONVERT(142,143,143));

            llFillSingleColor(x+3,y+3,x+9,y+9,RGB_CONVERT(50,50,50));
        }
    }
    else
    {
        if(widget->noCheckImagePath!=NULL)
        {

        }
        else
        {

            //画方框
            llFillSingleColor(x,y,x+12,y+12,RGB_CONVERT(244,244,244));
            llDrawLine(x,y,x+12,y,RGB_CONVERT(142,143,143));
            llDrawLine(x,y,x,y+12,RGB_CONVERT(142,143,143));
            llDrawLine(x+12,y,x+12,y+12,RGB_CONVERT(142,143,143));
            llDrawLine(x,y+12,x+12,y+12,RGB_CONVERT(142,143,143));

        }
    }
}

void pCheckBoxRefresh(llCheckBox *widget,bool toggle)
{
    llPoint globalPos;
    int16_t textHeight;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
        globalPos=llListGetGlobalPos(widget->parentWidget);
        if(toggle==false)//刷新
        {

            pCheckBoxShow(widget);
        }
        else//点击触发
        {

            if(widget->isChecked==false)
            {
                widget->isChecked=true;
                pCheckBoxShow(widget);

                //llEmitSignal(widget,SIGNAL_CLICK_CHECK_PRESSED);
            }
            else
            {
                widget->isChecked=false;
                pCheckBoxShow(widget);

                //llEmitSignal(widget,SIGNAL_CLICK_CHECK_RELASED);
            }
        }

        llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t *)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);
        textHeight=llFontGetHeightSize();

        //更新文字坐标
        widget->textInfo.geometry.x=widget->geometry.x+globalPos.x+12+5;
        widget->textInfo.geometry.y=widget->geometry.y+globalPos.y;
        widget->textInfo.geometry.width=widget->geometry.width-12-5;
        widget->textInfo.showGeometry=widget->textInfo.geometry;

        llCharDisplay(&(widget->textInfo));
    }
    }
}

void nCheckBoxRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeCheckBox);
    if(widget!=NULL)
    {
        pCheckBoxRefresh((llCheckBox*)widget,false);
    }
}

bool slotCheckBoxToggle(llConnectInfo info)
{
    llCheckBox * cb;

    cb=(llCheckBox*)info.receiver;

    pCheckBoxRefresh(cb,true);
    return false;
}

llCheckBox *llCheckBoxQuickCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,
                                uint8_t *text,llFontLib *fontLib,llColor textColor,
                                bool isChecked,bool isHidden)
{
    llCheckBox * pNewWidget = NULL;
    uint8_t *pText = NULL;
    llListWidgetInfo *parentInfo;
    uint32_t textLength=0;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llCheckBox);
        pText=(uint8_t *)llMalloc(LL_CHECKBOX_TEXT_LENGTH_MAX*sizeof(uint8_t));
        if((pNewWidget!=NULL)&&(pText!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeCheckBox;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->checkImagePath=NULL;
            pNewWidget->noCheckImagePath=NULL;

            pNewWidget->isHidden=isHidden;

            //add linked list

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);


            pNewWidget->isChecked=isChecked;
            pNewWidget->isSelected=false;
            pNewWidget->deleteFunc=nCheckBoxDelete;
            pNewWidget->actionFunc=llCheckBoxAction;
            pNewWidget->refreshFunc=nCheckBoxRefresh;
            pNewWidget->textInfo.backgroundColor=0;
            pNewWidget->textInfo.isTransparent=true;
            pNewWidget->textInfo.charColor=textColor;
            pNewWidget->textInfo.fontLibInfo=fontLib;
            pNewWidget->textInfo.geometry.x=x;
            pNewWidget->textInfo.geometry.y=y;
            pNewWidget->textInfo.geometry.width=width;
            pNewWidget->textInfo.geometry.height=height;
            pNewWidget->textInfo.hAlign=llAlignHLeft;
            pNewWidget->textInfo.vAlign=llAlignVCenter;
            pNewWidget->textInfo.text=pText;
            pNewWidget->isEnable=true;
            pNewWidget->textInfo.isAutoLineBreak=false;
            pNewWidget->textInfo.isPassword=false;

            textLength=strlen((const char*)text)+1;
            textLength=((textLength>LL_CHECKBOX_TEXT_LENGTH_MAX)?LL_CHECKBOX_TEXT_LENGTH_MAX:textLength);

            memcpy(pNewWidget->textInfo.text,text,textLength);
            //最后一个字节强制变0
            (pNewWidget->textInfo.text)[textLength-1]=0;

            pCheckBoxRefresh(pNewWidget,false);

        }
        else
        {
            llFree(pNewWidget);
            llFree(pText);
        }
    }

    //动作
    llConnectSignal(nameId,SIGNAL_CLICK_PRESSED,nameId,slotCheckBoxToggle);

    return pNewWidget;
}

llCheckBox *llCheckBoxCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,uint8_t *text,llFontLib *fontLib,bool isHidden)
{
    return llCheckBoxQuickCreate(nameId,parentNameId,x,y,width,height,text,fontLib,0,false,isHidden);
}


bool pCheckBoxGetChecked(llCheckBox *widget)
{
    return widget->isChecked;
}

bool nCheckBoxGetChecked(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeCheckBox);
    if(widget!=NULL)
    {
        return pCheckBoxGetChecked((llCheckBox*)widget);
    }
    return false;
}

void pCheckBoxSetChecked(llCheckBox *widget, bool state)
{
    if(widget->isEnable)
    {
        widget->isChecked=state;
        pCheckBoxRefresh((llCheckBox*)widget,false);
    }
}

void nCheckBoxSetChecked(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeCheckBox);
    if(widget!=NULL)
    {
        pCheckBoxSetChecked(widget,state);
    }
}

void pCheckBoxSetEnabled(llCheckBox *widget, bool state)
{
    widget->isEnable=state;
    pCheckBoxRefresh(widget,false);
}

void nCheckBoxSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeCheckBox);
    if(widget!=NULL)
    {
        pCheckBoxSetEnabled((llCheckBox*)widget,state);
    }
}

void pCheckBoxSetHidden(llCheckBox *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pCheckBoxRefresh(widget,false);
    }
}

void nCheckBoxSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeCheckBox);
    if(widget!=NULL)
    {
        pCheckBoxSetHidden(widget,isHidden);
    }
}


