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

#include "LL_LineEdit.h"
#include "LL_Linked_List.h"
#include "LL_User.h"
#include "LL_Characters.h"
#include "LL_Keyboard.h"

bool slotLineEditInputNum(llConnectInfo info)
{
    llLineEdit * lineEdit;

    lineEdit=(llLineEdit*)info.receiver;

    llKeyboardNumCreate(lineEdit,0, cfgMonitorHeight/2, cfgMonitorWidth, cfgMonitorHeight/2,lineEdit->keyboardFontLib);
    return false;
}

bool slotLineEditInputQwerty(llConnectInfo info)
{
    llLineEdit * lineEdit;

    lineEdit=(llLineEdit*)info.receiver;

    llKeyboardQwertyCreate(lineEdit,0, cfgMonitorHeight/2, cfgMonitorWidth, cfgMonitorHeight/2,lineEdit->keyboardFontLib);
    return false;
}

void pLineEditFree(llLineEdit *widget)
{
    llFree(widget->textInfo.text);
    llFree(widget);
}

void nLineEditDelete(uint16_t nameId)
{
    llListHead *temp_pos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llLineEdit *widget;

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
                pLineEditFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void pLineEditCursorBlink(llLineEdit *widget)
{
    int16_t x,y;
    uint8_t fontHeight=0;

    if(widget->isCursorEnable)
    {
        widget->isInput=true;

        llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t*)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);

        fontHeight=llFontGetHeightSize();
        x=widget->textInfo.geometry.x+widget->textInfo.textPointWidth;
        y=llCharCalVerticalPos(&widget->textInfo,fontHeight);

        llCharSetCursor(widget,&(widget->isInput),x,y,fontHeight,RGB888(0xffffff));
    }
}

void llLineEditAction(void *widget,uint8_t touchSignal)
{
    if(((llLineEdit*)widget)->isEnable)
    {
        if(touchSignal==SIGNAL_CLICK_PRESSED)
        {
            if(((llLineEdit*)widget)->isInput==false)
            {
                pLineEditCursorBlink(widget);
                llEmitSignal(widget,SIGNAL_WIDGET_ACTIVE);
            }
        }
        if((touchSignal==SIGNAL_CLICK_PRESSED)||(touchSignal==SIGNAL_CLICK_RELEASED))
        {
            llEmitSignal(widget,touchSignal);
        }
    }
}

void pLineEditRefresh(llLineEdit *widget)
{
//    int16_t offsetX = 0,offsetY = 0;
    int16_t globalX = 0,globalY = 0;
    int16_t width,height;
    llPoint globalPos;
    uint16_t textStrWidth=0;
    uint8_t strTemp=0;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);
            //更新文字坐标
            width=((llGeneral*)widget)->geometry.width;
            height=((llGeneral*)widget)->geometry.height;

            globalX=globalPos.x+widget->geometry.x;
            globalY=globalPos.y+widget->geometry.y;

            widget->textInfo.geometry.x=globalX+1;//减去边框
            widget->textInfo.geometry.y=globalY+1;//减去边框
            widget->textInfo.geometry.width=width-2;//减去边框
            widget->textInfo.geometry.height=height-2;//减去边框

            widget->textInfo.showGeometry=widget->textInfo.geometry;

            llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t*)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);

            //判断是否过长,过长截断,无视有效长度
            while(widget->textInfo.text[strTemp]!=0)
            {
                //英文 0x0-0x7F
                if(widget->textInfo.text[strTemp]<0x80)
                {
                    textStrWidth+=llFontGetWidthSize(&widget->textInfo.text[strTemp]);
                }
                else
                {
                    //utf8 chinese use 3 bytes
                    textStrWidth+=llFontGetWidthSize(&widget->textInfo.text[strTemp]);
                    strTemp++;
                    strTemp++;
                }
                strTemp++;
            }
            widget->textInfo.textPointWidth=textStrWidth;

            if(textStrWidth<=(widget->textInfo.geometry.width-3))//小于(内宽度+光标)
            {
                llCharDisplay(&(widget->textInfo));

                //画边框
                llFillSingleColor(globalX,globalY,globalX+width-1,globalY,RGB_CONVERT(171,173,179));
                llFillSingleColor(globalX,globalY,globalX,globalY+height-1,RGB_CONVERT(226,227,234));
                llFillSingleColor(globalX,globalY+height-1,globalX+width-1,globalY+height-1,RGB_CONVERT(227,233,239));
                llFillSingleColor(globalX+width-1,globalY,globalX+width-1,globalY+height-1,RGB_CONVERT(219,223,230));

                if(widget->isInput==true)
                {
                    pLineEditCursorBlink(widget);
                }
            }
            else
            {
                //英文 0x0-0x7F
                if((widget->textInfo.text[strTemp-1]<0x80)&&(widget->textInfo.text[strTemp-3]<0x80))
                {
                    widget->textInfo.text[strTemp-1]=0;
                }
                else
                {
                    widget->textInfo.text[strTemp-1]=0;
                    widget->textInfo.text[strTemp-2]=0;
                    widget->textInfo.text[strTemp-3]=0;
                }
            }
        }
    }
}

void nLineEditRefresh(uint16_t nameId)
{
    llListWidgetInfo *widgetInfo;

    if(llList_GetInfoByName(&widgetInfo,nameId)==true)
    {
        if(((llLineEdit*)widgetInfo->widget)->widgetType==widgetTypeLineEdit)
        {
            pLineEditRefresh((llLineEdit*)widgetInfo->widget);
        }
    }
}

llLineEdit *llLineEditQuickCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,
                                  uint8_t *text,llFontLib *fontLib,llColor textColor,
                                  llFontLib *keyboardFontLib,
                                  bool isHidden)
{
    llLineEdit * pNewWidget = NULL;
    uint8_t *pText = NULL;
    llListWidgetInfo *parentInfo;
    uint32_t textLength=0;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llLineEdit);
        pText=(uint8_t*)llMalloc(sizeof (uint8_t)*LL_LINEEDIT_TEXT_LENGTH_MAX);

        if((pNewWidget!=NULL)&&(pText!=NULL))
        {
            memset(pText,0,LL_LINEEDIT_TEXT_LENGTH_MAX);
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeLineEdit;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;
            pNewWidget->textMaxLength=LL_LINEEDIT_TEXT_LENGTH_MAX;
            pNewWidget->keyboardFontLib=keyboardFontLib;
            pNewWidget->isHidden=isHidden;
            pNewWidget->isInput=false;
            pNewWidget->isOnlyInputNum=false;
            pNewWidget->isEnable=true;
            pNewWidget->keyboardType=0;
            pNewWidget->isNum=false;
            pNewWidget->minValue=0;
            pNewWidget->maxValue=100;

            //add linked list

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            pNewWidget->deleteFunc=nLineEditDelete;
            pNewWidget->actionFunc=llLineEditAction;
            pNewWidget->refreshFunc=nLineEditRefresh;
            pNewWidget->textInfo.backgroundColor=RGB888(0xFFFFFF);
            pNewWidget->textInfo.isTransparent=false;
            pNewWidget->textInfo.charColor=textColor;
            pNewWidget->textInfo.fontLibInfo=fontLib;

            pNewWidget->textInfo.geometry.x=((llGeneral*)(parentInfo->widget))->geometry.x+x+1;//减去边框
            pNewWidget->textInfo.geometry.y=((llGeneral*)(parentInfo->widget))->geometry.y+y+1;//减去边框
            pNewWidget->textInfo.geometry.width=width-2;//减去边框
            pNewWidget->textInfo.geometry.height=height-2;//减去边框
            pNewWidget->textInfo.hAlign=llAlignHLeft;
            pNewWidget->textInfo.vAlign=llAlignVCenter;
            pNewWidget->textInfo.text=pText;
            pNewWidget->textInfo.isAutoLineBreak=false;
            pNewWidget->textInfo.isPassword=false;
            pNewWidget->isCursorEnable=true;

            textLength=strlen((const char*)text)+1;
            textLength=((textLength>LL_LINEEDIT_TEXT_LENGTH_MAX)?LL_LINEEDIT_TEXT_LENGTH_MAX:textLength);

            memcpy(pNewWidget->textInfo.text,text,textLength);
            //最后一个字节强制变0
            (pNewWidget->textInfo.text)[textLength-1]=0;

            pLineEditRefresh(pNewWidget);
        }
        else
        {
            llFree(pNewWidget);
            llFree(pText);
        }
    }

    return pNewWidget;
}

llLineEdit *llLineEditCreate(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint8_t *text,llFontLib *fontLib,llFontLib *keyboardFontLib,bool isHidden)
{
    return llLineEditQuickCreate(nameId,parentNameId,x,y,width,height,text,fontLib,0,keyboardFontLib,isHidden);
}

void pLineEditSetText(llLineEdit *widget,uint8_t *text)
{
    uint16_t textLength;
    float fTemp;
    bool isInputEnable=true;

    if(widget->isEnable)
    {
        if(widget->isNum)
        {
            fTemp=llStrToFloat(text);

            if(!((fTemp>=(widget->minValue))&&(fTemp<=(widget->maxValue))))
            {
                isInputEnable=false;
            }
        }

        if(isInputEnable)
        {
            textLength=strlen((const char*)text)+1;
            textLength=((textLength>LL_LINEEDIT_TEXT_LENGTH_MAX)?LL_LINEEDIT_TEXT_LENGTH_MAX:textLength);
            memcpy((widget)->textInfo.text,text,textLength);
            //最后一个字节强制变0
            ((widget)->textInfo.text)[textLength-1]=0;

            pLineEditRefresh(widget);

            llEmitSignal(widget,SIGNAL_VALUE_CHANGED);
        }
    }
}

void nLineEditSetText(uint16_t nameId,uint8_t *text)
{
    void* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetText(widget,text);
    }
}

uint8_t* pLineEditGetText(llLineEdit *widget)
{
    return widget->textInfo.text;
}

uint8_t* nLineEditGetText(uint16_t nameId)
{
    void* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        return pLineEditGetText((llLineEdit*)widget);
    }
    return 0;
}

void pLineEditSetSelect(llLineEdit *widget,bool state)
{
    if(widget->isEnable)
    {
        if((state==false)&&(widget->isInput==true))
        {
            llCharStopCursorBlink();
        }
        widget->isInput=state;
        pLineEditRefresh(widget);
    }
}

void nLineEditSetSelect(uint16_t nameId,bool state)
{
    void* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetSelect((llLineEdit*)widget,state);
    }
}


void pLineEditSetEnabled(llLineEdit *widget, bool state)
{
    widget->isEnable=state;
    if(widget->isEnable==false)
    {
        llCharStopCursorBlink();
    }
}

void nLineEditSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetEnabled((llLineEdit*)widget,state);
    }
}

void pLineEditSetHidden(llLineEdit *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pLineEditRefresh(widget);
    }
}

void nLineEditSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetHidden(widget,isHidden);
    }
}

void pLineEditSetCursorEnable(llLineEdit *widget,bool state)
{
    if(widget->isEnable)
    {
        widget->isCursorEnable=state;
        if(widget->isCursorEnable==false)
        {
            llCharStopCursorBlink();
        }
    }
}

void nLineEditSetCursorEnable(uint16_t nameId,bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetCursorEnable(widget,state);
    }
}

void nLineEditSetKeyboard(uint16_t nameId,uint8_t keyboardType)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        ((llLineEdit*)widget)->keyboardType=keyboardType;
        switch(keyboardType)
        {
        case KEYBOARD_NUM:
        {
            llDisconnectSignal(nameId,SIGNAL_WIDGET_ACTIVE,nameId,slotLineEditInputQwerty);
            llConnectSignal(nameId,SIGNAL_WIDGET_ACTIVE,nameId,slotLineEditInputNum);
            break;
        }
        case KEYBOARD_QWERTY:
        {
            llDisconnectSignal(nameId,SIGNAL_WIDGET_ACTIVE,nameId,slotLineEditInputNum);
            llConnectSignal(nameId,SIGNAL_WIDGET_ACTIVE,nameId,slotLineEditInputQwerty);
            break;
        }
        default:
        {
            llDisconnectSignal(nameId,SIGNAL_WIDGET_ACTIVE,nameId,slotLineEditInputNum);
            llDisconnectSignal(nameId,SIGNAL_WIDGET_ACTIVE,nameId,slotLineEditInputQwerty);
            break;
        }
        }
    }
}

void pLineEditSetMinNum(llLineEdit *widget,float value)
{
    if(widget->isEnable)
    {
        widget->isNum=true;
        widget->minValue=value;
    }
}

void nLineEditSetMinNum(uint16_t nameId,float value)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetMinNum(widget,value);
    }
}

void pLineEditSetMaxNum(llLineEdit *widget,float value)
{
    if(widget->isEnable)
    {
        widget->isNum=true;
        widget->maxValue=value;
    }
}

void nLineEditSetMaxNum(uint16_t nameId,float value)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditSetMaxNum(widget,value);
    }
}

void pLineEditPasswordEnable(llLineEdit *widget,bool enable)
{
    if(widget->isEnable)
    {
        widget->textInfo.isPassword=enable;
    }
}

void nLineEditPasswordEnable(uint16_t nameId,bool enable)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeLineEdit);
    if(widget!=NULL)
    {
        pLineEditPasswordEnable(widget,enable);
    }
}

