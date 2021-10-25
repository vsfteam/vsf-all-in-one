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

#include "LL_Text.h"
#include "LL_Linked_List.h"
#include "LL_Characters.h"
//#include "LL_LatticeLib.h"
#include "LL_User.h"
#include "LL_Window.h"
#include "LL_Background.h"

void pTextFree(llText *widget)
{
    llFree(widget->textInfo.text);
    llFree(widget);
}

void nTextDelete(uint16_t nameId)
{
    llListHead *temp_pos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llText *widget;

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
                pTextFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llTextAction(void *widget,uint8_t touchSignal)
{
    bool clickState;
    llPoint pos;
    if(((llText*)widget)->isEnable)
    {
        switch(touchSignal)
        {
        case SIGNAL_CLICK_PRESSED:
        {
            //限定top显示才有效
            if((((llText*)widget)->textInfo.vAlign==llAlignVTop)&&(((llText*)widget)->isLineBreak))
            {
                clickState=llClickGetPoint(&pos.x,&pos.y);
                if(clickState==true)
                {
                    ((llText*)widget)->sysClickedPos=pos;
                    ((llText*)widget)->offsetVertical=((llText*)widget)->moveVertical;
                }
            }
            llEmitSignal(widget,touchSignal);
            break;
        }
        case SIGNAL_CLICK_RELEASED:
        {
            //限定top显示才有效
            if((((llText*)widget)->textInfo.vAlign==llAlignVTop)&&(((llText*)widget)->isLineBreak))
            {
                if(((llText*)widget)->sysGeometry.y>((llText*)widget)->geometry.y)
                {
                    ((llText*)widget)->moveVertical=0;
                    pTextRefresh(((llText*)widget));
                }
                if((((llText*)widget)->sysGeometry.y+((llText*)widget)->sysGeometry.height)<(((llText*)widget)->geometry.y+((llText*)widget)->geometry.height))
                {
                    ((llText*)widget)->moveVertical=((llText*)widget)->geometry.height-((llText*)widget)->sysGeometry.height;
                    pTextRefresh(((llText*)widget));
                }
            }
            llEmitSignal(widget,touchSignal);
        }
        case SIGNAL_CLICK_HOLD_MOVE:
        {
            //限定top显示才有效
            if((((llText*)widget)->textInfo.vAlign==llAlignVTop)&&(((llText*)widget)->isLineBreak))
            {
                llEmitSignal(widget,touchSignal);
            }
            break;
        }
        default:
            break;
        }
    }
}

void pTextRefresh(llText *widget)
{
    llPoint globalPos;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);
            //更新文字坐标
            widget->textInfo.showGeometry.x=widget->geometry.x+globalPos.x;
            widget->textInfo.showGeometry.y=widget->geometry.y+globalPos.y;

            if(widget->textInfo.isTransparent)
            {
                llGeneralWidgetParentRecover((llGeneral*)widget,widget->textInfo.showGeometry);
            }

            widget->textInfo.geometry.x=widget->textInfo.showGeometry.x+widget->moveHorizontal;
            widget->textInfo.geometry.y=widget->textInfo.showGeometry.y+widget->moveVertical;

            llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t *)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);
            widget->sysGeometry=llCharDisplay(&(widget->textInfo));
        }
    }
}

void nTextRefresh(uint16_t nameId)
{
    llListWidgetInfo *widgetInfo;

    if(llList_GetInfoByName(&widgetInfo,nameId)==true)
    {
        if(((llText*)widgetInfo->widget)->widgetType==widgetTypeText)
        {
            pTextRefresh((llText*)widgetInfo->widget);
        }
    }
}

bool slotTextMoveVertical(llConnectInfo info)
{
    llText * text;
    bool clickState;
    llPoint pos;

    text=(llText*)info.sender;
    //限定top显示才有效
    if(text->textInfo.vAlign==llAlignVTop)
    {

        //判断是否允许移动,文本高度大于于显示高度
        if(text->sysGeometry.height>text->textInfo.geometry.height)
        {
            clickState=llClickGetPoint(&pos.x,&pos.y);
            text->moveVertical=pos.y-text->sysClickedPos.y+text->offsetVertical;
            pTextRefresh(text);
        }
    }
    return false;
}

bool isLineBreak(uint8_t *text)
{
    uint16_t textLen;
    textLen=strlen((const char*)text);
    while(textLen--)
    {
        if(text[textLen-1]=='\n')
        {
            return true;
        }
    }
    return false;
}

llText *llTextQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                          uint8_t *text,llFontLib *fontLib,llColor textColor,llColor bgColor,
                          llHorizontalAlign hAlign,llVerticalAlign vAlign,
                          int16_t moveHorizontal,int16_t moveVertical,
                          bool isAutoLineBreak,bool isTransparent,bool isHidden)
{
    llText * pNewWidget = NULL;
    uint8_t *pText = NULL;
    llListWidgetInfo *parentInfo;



    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llText);
        pText=LL_MALLOC_STRING(text);
        if((pNewWidget!=NULL)&&(pText!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeText;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;

            pNewWidget->moveVertical=moveVertical;
            pNewWidget->moveHorizontal=moveHorizontal;

            //add linked list

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            pNewWidget->deleteFunc=nTextDelete;
            pNewWidget->actionFunc=llTextAction;
            pNewWidget->refreshFunc=nTextRefresh;
            pNewWidget->textInfo.backgroundColor=bgColor;
            pNewWidget->textInfo.isTransparent=isTransparent;
            pNewWidget->textInfo.charColor=textColor;
            pNewWidget->textInfo.fontLibInfo=fontLib;
//            pNewWidget->textInfo.geometry.x=((llGeneral*)(parentInfo->widget))->geometry.x+x;
//            pNewWidget->textInfo.geometry.y=((llGeneral*)(parentInfo->widget))->geometry.y+y;
            pNewWidget->textInfo.geometry.width=width;
            pNewWidget->textInfo.geometry.height=height;
            pNewWidget->textInfo.showGeometry.width=width;
            pNewWidget->textInfo.showGeometry.height=height;
            pNewWidget->textInfo.hAlign=hAlign;
            pNewWidget->textInfo.vAlign=vAlign;
            pNewWidget->textInfo.text=pText;
            pNewWidget->textInfo.mallocLength=strlen((const char*)text);
            pNewWidget->textInfo.isAutoLineBreak=isAutoLineBreak;
            strcpy((char *)pNewWidget->textInfo.text,(const char *)text);

            pNewWidget->textInfo.isPassword=false;
            pNewWidget->textInfo.charColor=textColor;

            pNewWidget->isLineBreak= isLineBreak(pText);
            pTextRefresh(pNewWidget);

            llConnectSignal(nameId,SIGNAL_CLICK_HOLD_MOVE,nameId,slotTextMoveVertical);
        }
        else
        {
            llFree(pNewWidget);
            llFree(pText);
        }
    }

    return pNewWidget;
}

llText *llTextCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t *text,llFontLib *fontLib,bool isHidden)
{
    return llTextQuickCreate(nameId,parentNameId,x,y,width,height,text,fontLib,0,RGB_CONVERT(255,255,255),llAlignHLeft,llAlignVCenter,0,0,false,true,isHidden);
}

void pTextSetTransparent(llText *widget,bool isTransparent)
{
    if(widget->isEnable)
    {
        widget->textInfo.isTransparent=isTransparent;
        pTextRefresh(widget);
    }
}

void nTextSetTransparent(uint16_t nameId,bool isTransparent)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetTransparent((llText*)widget,isTransparent);
    }
}

void pTextSetFont(llText *widget,llFontLib *fontLib)
{
    if(widget->isEnable)
    {
        widget->textInfo.fontLibInfo=fontLib;
        pTextRefresh(widget);
    }
}

void nTextSetFont(uint16_t nameId,llFontLib *fontLib)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetFont((llText*)widget,fontLib);
    }
}

void pTextSetTextColor(llText *widget,llColor color)
{
    if(widget->isEnable)
    {
        widget->textInfo.charColor=color;
        pTextRefresh(widget);
    }
}

void nTextSetTextColor(uint16_t nameId,llColor color)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetTextColor((llText*)widget,color);
    }
}

void pTextSetBackgroundColor(llText *widget,llColor color)
{
    if(widget->isEnable)
    {
        widget->textInfo.isTransparent=false;
        widget->textInfo.backgroundColor=color;
        pTextRefresh(widget);
    }
}

void nTextSetBackgroundColor(uint16_t nameId,llColor color)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetBackgroundColor((llText*)widget,color);
    }
}

void pTextRecoverBackground(llText *widget,llGeometry geometry)
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

void pTextSetText(llText *widget,uint8_t *text)
{
    uint8_t *pText;
    uint16_t textLen;
    llPoint globalPos;
    llGeometry recoverGeometry;
    if(widget->isEnable)
    {
        textLen=strlen((const char*)text);
        if(widget->textInfo.mallocLength<textLen)
        {
            llFree(widget->textInfo.text);
            pText=LL_MALLOC_STRING(text);
            widget->textInfo.mallocLength=textLen;
        }
        else
        {
            pText=widget->textInfo.text;
        }
        strcpy((char *)pText,(const char *)text);
        widget->textInfo.text=pText;
        
        widget->isLineBreak= isLineBreak(pText);

        globalPos=llListGetGlobalPos(widget->parentWidget);
        recoverGeometry=widget->geometry;
        recoverGeometry.x+=globalPos.x;
        recoverGeometry.y+=globalPos.y;

        pTextRecoverBackground(widget,recoverGeometry);
        pTextRefresh(widget);
    }
}

void nTextSetText(uint16_t nameId,uint8_t *text)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetText((llText*)widget,text);
    }
}

uint8_t* pTextGetText(llText *widget)
{
    return widget->textInfo.text;
}

uint8_t* nTextGetText(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        return pTextGetText((llText*)widget);
    }
    return 0;
}

void pTextSetAlign(llText *widget,llHorizontalAlign hAlign,llVerticalAlign vAlign)
{
    if(widget->isEnable)
    {
        widget->textInfo.hAlign=hAlign;
        widget->textInfo.vAlign=vAlign;
        pTextRefresh(widget);
    }
}

void nTextSetAlign(uint16_t nameId,llHorizontalAlign hAlign,llVerticalAlign vAlign)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetAlign((llText*)widget,hAlign,vAlign);
    }
}


void pTextSetEnabled(llText *widget, bool state)
{
    widget->isEnable=state;
}

void nTextSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetEnabled((llText*)widget,state);
    }
}

void pTextSetHidden(llText *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pTextRefresh(widget);
    }
}

void nTextSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeText);
    if(widget!=NULL)
    {
        pTextSetHidden(widget,isHidden);
    }
}
