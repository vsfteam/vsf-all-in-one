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

#include "LL_Button.h"
#include "LL_Linked_List.h"
//#include "LL_LatticeLib.h"
#include "LL_Background.h"
#include "LL_User.h"
#include "LL_Window.h"



//void pButtonShowStyle(llButton *widget,int16_t x,int16_t y,int16_t width,int16_t height)
//{
//    uint16_t i,j,temp;

//    if(widget->isPressed)
//    {
//        //shadow
//        for(i=0; i<height; i++)
//        {
//            llDrawLine(x,y+i,x+width,y+i,RGB_CONVERT(226,225,217));
//        }

//        //4 line
//        llDrawLine(x+3,y,x+width-3,y,RGB_CONVERT(0,60,116));
//        llDrawLine(x+3,y+height,x+width-3,y+height,RGB_CONVERT(0,60,116));
//        llDrawLine(x,y+3,x,y+height-3,RGB_CONVERT(0,60,116));
//        llDrawLine(x+width,y+3,x+width,y+height-3,RGB_CONVERT(0,60,116));

//        //4 corner
//        llSetPoint(x+1,y,RGB_CONVERT(122,149,168));
//        llSetPoint(x,y+1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+2,y,RGB_CONVERT(37,87,131));
//        llSetPoint(x,y+2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+1,y+1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+2,y+1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+1,y+2,RGB_CONVERT(192,207,221));

//        llSetPoint(x+width-1,y,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width,y+1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width-2,y,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width,y+2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width-1,y+1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+width-2,y+1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+width-1,y+2,RGB_CONVERT(192,207,221));

//        llSetPoint(x+1,y+height,RGB_CONVERT(122,149,168));
//        llSetPoint(x,y+height-1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+2,y+height,RGB_CONVERT(37,87,131));
//        llSetPoint(x,y+height-2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+1,y+height-1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+2,y+height-1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+1,y+height-2,RGB_CONVERT(192,207,221));

//        llSetPoint(x+width-1,y+height,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width,y+height-1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width-2,y+height,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width,y+height-2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width-1,y+height-1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+width-2,y+height-1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+width-1,y+height-2,RGB_CONVERT(192,207,221));

//        //top
//        llDrawLine(x+3,y+1,x+width-3,y+1,RGB_CONVERT(189,184,173));
//        llDrawLine(x+3,y+2,x+width-3,y+2,RGB_CONVERT(209,204,193));
//        //bottom

//        llDrawLine(x+3,y+height-1,x+width-3,y+height-1,RGB_CONVERT(242,241,238));
//        //left
//        llDrawLine(x+1,y+3,x+1,y+height-3,RGB_CONVERT(195,192,182));
//        llDrawLine(x+2,y+3,x+2,y+height-3,RGB_CONVERT(215,212,202));
//        //right
//        llDrawLine(x+width-1,y+3,x+width-1,y+height-3,RGB_CONVERT(228,227,219));
//    }
//    else
//    {
//        //shadow
//        for(i=1; i<width; i++)
//        {
//            for(j=0; j<(height-4); j++)
//            {
//                temp=height/15;
//                temp=j*2/temp;
//                llSetPoint(x+i,y+j+1,RGB_CONVERT((255-temp),(255-temp),(255-temp)));//背景渐变过度色
//            }
//        }

//        //4 line
//        llDrawLine(x+3,y,x+width-3,y,RGB_CONVERT(0,60,116));
//        llDrawLine(x+3,y+height,x+width-3,y+height,RGB_CONVERT(0,60,116));
//        llDrawLine(x,y+3,x,y+height-3,RGB_CONVERT(0,60,116));
//        llDrawLine(x+width,y+3,x+width,y+height-3,RGB_CONVERT(0,60,116));

//        //4 corner
//        llSetPoint(x+1,y,RGB_CONVERT(122,149,168));
//        llSetPoint(x,y+1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+2,y,RGB_CONVERT(37,87,131));
//        llSetPoint(x,y+2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+1,y+1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+2,y+1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+1,y+2,RGB_CONVERT(192,207,221));

//        llSetPoint(x+width-1,y,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width,y+1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width-2,y,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width,y+2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width-1,y+1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+width-2,y+1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+width-1,y+2,RGB_CONVERT(192,207,221));

//        llSetPoint(x+1,y+height,RGB_CONVERT(122,149,168));
//        llSetPoint(x,y+height-1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+2,y+height,RGB_CONVERT(37,87,131));
//        llSetPoint(x,y+height-2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+1,y+height-1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+2,y+height-1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+1,y+height-2,RGB_CONVERT(192,207,221));

//        llSetPoint(x+width-1,y+height,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width,y+height-1,RGB_CONVERT(122,149,168));
//        llSetPoint(x+width-2,y+height,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width,y+height-2,RGB_CONVERT(37,87,131));
//        llSetPoint(x+width-1,y+height-1,RGB_CONVERT(85,125,62));
//        llSetPoint(x+width-2,y+height-1,RGB_CONVERT(192,207,221));
//        llSetPoint(x+width-1,y+height-2,RGB_CONVERT(192,207,221));

//        //bottom
//        llDrawLine(x+1,y+height-3,x+width-1,y+height-3,RGB_CONVERT(216,215,210));
//        llDrawLine(x+2,y+height-2,x+width-2,y+height-2,RGB_CONVERT(206,203,194));
//        llDrawLine(x+3,y+height-1,x+width-3,y+height-1,RGB_CONVERT(194,188,177));
//        //right
//        llDrawLine(x+width-2,y+2,x+width-2,y+height-2,RGB_CONVERT(218,217,210));
//        llDrawLine(x+width-1,y+3,x+width-1,y+height-3,RGB_CONVERT(212,209,212));
//    }
//}

void pButtonShow(llButton *widget)
{
    int16_t x,y,width,height;

    llPoint globalPos;
    globalPos=llListGetGlobalPos(widget->parentWidget);

    x=widget->geometry.x+globalPos.x;
    y=widget->geometry.y+globalPos.y;
    width=widget->geometry.width;
    height=widget->geometry.height;

    switch (widget->disType)
    {
    case BUTTON_DIS_TYPE_TRANSPARENT://透明
    {
        break;
    }
    case BUTTON_DIS_TYPE_COLOR://纯色
    {
        if(widget->isPressed)
        {
            llFillSingleColor(x,y,x+width-1,y+height-1,widget->pressedColor);
            if(widget->isSelected)//被选中
            {
                llGeneralWidgetSelect((llGeneral*)widget);
            }
        }
        else
        {
            llFillSingleColor(x,y,x+width-1,y+height-1,widget->releasedColor);
            if(widget->isSelected)//被选中
            {
                llGeneralWidgetSelect((llGeneral*)widget);
            }
        }

        break;
    }
    case BUTTON_DIS_TYPE_IMG://图片
    {
        if(widget->isPressed)
        {
            llGeneralImageShow((llGeneral*)widget,widget->pressedImageAddr,x,y);
            if(widget->isSelected)//被选中
            {
                llGeneralWidgetSelect((llGeneral*)widget);
            }
        }
        else
        {
            llGeneralImageShow((llGeneral*)widget,widget->releasedImageAddr,x,y);
            if(widget->isSelected)//被选中
            {
                llGeneralWidgetSelect((llGeneral*)widget);
            }
        }

        break;
    }
//    case BUTTON_DIS_TYPE_STYLE:
//    {
//        pButtonShowStyle(widget,x,y,width,height);
//        break;
//    }
    default:
        break;
    }
}



void pButtonToggle(llButton *widget)
{
    if(widget->isEnable)
    {
        if(widget->isPressed)
        {
            ((llButton*)widget)->isPressed=false;
            pButtonShow(widget);

        }
        else
        {
            ((llButton*)widget)->isPressed=true;
            pButtonShow(widget);

        }
    }
}

void pButtonRefresh(llButton *widget,bool toggle)
{
    llPoint globalPos;
    bool isBeep=false;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);
            if(toggle==false)//刷新
            {
                pButtonShow(widget);
            }
            else//点击触发
            {
                if(widget->isCheckable==false)
                {
                    pButtonToggle(widget);
                    if((widget->isPressed)&&(widget->buzzerCfg==BUTTON_BUZZER_PRESSED))
                    {
                        isBeep=true;
                    }
                    else
                    {
                        if((widget->isPressed==false)&&(widget->buzzerCfg==BUTTON_BUZZER_RELEASED))
                        {
                            isBeep=true;
                        }
                    }
                    if(widget->buzzerCfg==BUTTON_BUZZER_ALL)
                    {
                        isBeep=true;
                    }
                }
                else
                {
                    if(widget->isPressed)
                    {
                        if(widget->isChecked==false)
                        {
                            widget->isChecked=true;
                            widget->isPressed=false;
                            llEmitSignal(widget,SIGNAL_CLICK_CHECK_PRESSED);
                        }
                        else
                        {
                            pButtonToggle(widget);
                            widget->isChecked=false;
                            llEmitSignal(widget,SIGNAL_CLICK_CHECK_RELEASED);
                            if((widget->buzzerCfg==BUTTON_BUZZER_RELEASED)||(widget->buzzerCfg==BUTTON_BUZZER_ALL))
                            {
                                isBeep=true;
                            }
                        }
                    }
                    else
                    {
                        if(widget->isChecked==false)
                        {
                            pButtonToggle(widget);
                            if((widget->buzzerCfg==BUTTON_BUZZER_PRESSED)||(widget->buzzerCfg==BUTTON_BUZZER_ALL))
                            {
                                isBeep=true;
                            }
                        }
                        else
                        {
                            widget->isPressed=true;
                        }
                    }
                }
                
                //触发声音
                if(isBeep)
                {
                    llBuzzerBeep();
                }
            }

            //更新文字坐标

            widget->textInfo.geometry.x=widget->geometry.x+globalPos.x;
            widget->textInfo.geometry.y=widget->geometry.y+globalPos.y;

            widget->textInfo.showGeometry=widget->textInfo.geometry;

            llCharOpenLibrary(widget->textInfo.fontLibInfo->libType,(uint8_t*)widget->textInfo.fontLibInfo->name,widget->textInfo.fontLibInfo->fontSize);
            llCharDisplay(&(widget->textInfo));
        }
        else
        {
            //如果按键触发父控件隐藏，在父链有隐藏的情况下，按键状态切换但不刷新
            if(toggle==true)
            {
                if(widget->isCheckable==false)
                {
                    widget->isPressed=!widget->isPressed;
                }
            }
        }
    }
}

void pButtonSetGeometry(llButton *widget,int16_t x,int16_t y,int16_t width,int16_t height)
{
    if(widget->isEnable)
    {
        widget->geometry.x=x;
        widget->geometry.y=y;
        widget->geometry.width=width;
        widget->geometry.height=height;
        pButtonRefresh(widget,false);
    }
}

void nButtonSetGeometry(uint16_t nameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    void* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetGeometry(((llButton*)widget),x,y,width,height);
    }
}

bool slotButtonToggle(llConnectInfo info)
{
    llButton * btn;

    btn=(llButton*)info.receiver;

    pButtonRefresh(btn,true);
    
    return false;
}


void pButtonFree(llButton *widget)
{
    llFree(widget->textInfo.text);
    llFree(widget);
}

void nButtonDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llButton *widget;

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
                pButtonFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llButtonAction(void *widget,uint8_t touchSignal)
{
    if((((llButton*)widget)->isEnable)&&(((llButton*)widget)->isHidden==false))
    {
        if((touchSignal==SIGNAL_CLICK_PRESSED)||(touchSignal==SIGNAL_CLICK_RELEASED))
        {
            llEmitSignal(widget,touchSignal);
        }
    }
}

llButton *llButtonQuickCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,
                            uint8_t *text,llFontLib *fontLib,llColor textColor,
                            llColor bgReleasedColor,llColor bgRressedColor,uint32_t bgReleasedImageAddr,uint32_t bgPressedImageAddr,
                            uint8_t disType,bool isHidden)
{
    llButton * pNewWidget = NULL;
    uint8_t *pText = NULL;
    llListWidgetInfo *parentInfo;
    uint32_t textLength=0;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llButton);
        pText=(uint8_t *)llMalloc(LL_BUTTON_TEXT_LENGTH_MAX*sizeof(uint8_t));
        if((pNewWidget!=NULL)&&(pText!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeButton;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->isEnable=true;

            pNewWidget->pressedColor=bgRressedColor;
            pNewWidget->releasedColor=bgReleasedColor;


            pNewWidget->releasedImageAddr=bgReleasedImageAddr;
            pNewWidget->pressedImageAddr=bgPressedImageAddr;

            pNewWidget->isHidden=isHidden;
            pNewWidget->keyValue=0;

            pNewWidget->disType=disType;

            //add linked list

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            pNewWidget->buzzerCfg=BUTTON_BUZZER_NONE;
            pNewWidget->isCheckable=false;
            pNewWidget->isChecked=false;
            pNewWidget->isPressed=false;
            pNewWidget->isSelected=false;
            pNewWidget->deleteFunc=nButtonDelete;
            pNewWidget->actionFunc=llButtonAction;
            pNewWidget->refreshFunc=nButtonRefresh;
            pNewWidget->textInfo.backgroundColor=0;
            pNewWidget->textInfo.isTransparent=true;
            pNewWidget->textInfo.charColor=textColor;
            pNewWidget->textInfo.fontLibInfo=fontLib;
            pNewWidget->textInfo.geometry.x=x;
            pNewWidget->textInfo.geometry.y=y;
            pNewWidget->textInfo.geometry.width=width;
            pNewWidget->textInfo.geometry.height=height;
            pNewWidget->textInfo.hAlign=llAlignHCenter;
            pNewWidget->textInfo.vAlign=llAlignVCenter;
            pNewWidget->textInfo.text=pText;
            pNewWidget->textInfo.isAutoLineBreak=false;
            pNewWidget->textInfo.isPassword=false;
            textLength=strlen((const char*)text)+1;
            textLength=((textLength>LL_BUTTON_TEXT_LENGTH_MAX)?LL_BUTTON_TEXT_LENGTH_MAX:textLength);

            memcpy(pNewWidget->textInfo.text,text,textLength);
            //最后一个字节强制变0
            (pNewWidget->textInfo.text)[textLength-1]=0;

            pButtonRefresh(pNewWidget,false);

            //动作
            llConnectSignal(nameId,SIGNAL_CLICK_PRESSED,nameId,slotButtonToggle);
            llConnectSignal(nameId,SIGNAL_CLICK_RELEASED,nameId,slotButtonToggle);
        }
        else
        {
            llFree(pNewWidget);
            llFree(pText);
        }
    }
    return pNewWidget;
}

llButton *llButtonCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,uint8_t *text,llFontLib *fontLib,bool isHidden)
{
    return llButtonQuickCreate(nameId,parentNameId,x,y,width,height,text,fontLib,0,0,0,NULL,NULL,BUTTON_DIS_TYPE_TRANSPARENT,isHidden);
}

//fontLib必须指针导入，否则数据会自动复制到另外一个空间供函数使用，地址就变化了
void pButtonSetFont(llButton *widget,llFontLib *fontLib)
{
    if(widget->isEnable)
    {
        widget->textInfo.fontLibInfo=fontLib;
        pButtonRefresh(widget,false);
    }
}

//fontLib必须指针导入，否则数据会自动复制到另外一个空间供函数使用，地址就变化了
void nButtonSetFont(uint16_t nameId,llFontLib *fontLib)
{
    void* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetFont((llButton*)widget,fontLib);
    }
}

void pButtonSetTextColor(llButton *widget,llColor color)
{
    if(widget->isEnable)
    {
        widget->textInfo.charColor=color;
        pButtonRefresh(widget,false);
    }
}

void nButtonSetTextColor(uint16_t nameId, llColor color)
{
    void* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetTextColor((llButton*)widget,color);
    }
}

void pButtonSetBackgroundColor(llButton *widget,llColor releasedColor,llColor pressedColor)
{
    if(widget->isEnable)
    {
        widget->pressedColor=pressedColor;
        widget->releasedColor=releasedColor;

        widget->disType=BUTTON_DIS_TYPE_COLOR;

        pButtonRefresh(widget,false);
    }
}

void nButtonSetBackgroundColor(uint16_t nameId,llColor releasedColor,llColor pressedColor)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetBackgroundColor((llButton*)widget,releasedColor,pressedColor);
    }
}

void pButtonSetBackgroundImage(llButton *widget,uint32_t releasedImageAddr,uint32_t pressedImageAddr)
{
    if(widget->isEnable)
    {
        widget->pressedColor=0;
        widget->releasedColor=0;
        widget->disType=BUTTON_DIS_TYPE_IMG;
        widget->pressedImageAddr=pressedImageAddr;
        widget->releasedImageAddr=releasedImageAddr;

        pButtonRefresh(widget,false);
    }
}

void nButtonSetBackgroundImage(uint16_t nameId,uint32_t releasedImageAddr,uint32_t pressedImageAddr)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetBackgroundImage((llButton*)widget,releasedImageAddr,pressedImageAddr);
    }
}

void pButtonSetStyle(llButton *widget)
{
    if(widget->isEnable)
    {
        widget->disType=BUTTON_DIS_TYPE_STYLE;

        pButtonRefresh(widget,false);
    }
}

void nButtonSetStyle(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetStyle((llButton*)widget);
    }
}

static bool slotJumpPage(llConnectInfo info)
{
    llBackground * bg=(llBackground*)info.receiver;
    llButton * btn=(llButton*)info.sender;

    llHandlerSetJumpPage(btn->jumpPage,bg->nameId);
    return false;
}

void nButtonSetJumpPage(uint16_t nameId,uint16_t targetPageNum)
{
    void *widget,*bgWidget;

    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    bgWidget=llListGetRootWidget();
    if((widget!=NULL)&&(bgWidget!=NULL))
    {
        ((llButton*)widget)->jumpPage=targetPageNum;
        llConnectSignal(nameId,SIGNAL_CLICK_PRESSED,((llBackground*)bgWidget)->nameId,slotJumpPage);
    }
}

void pButtonSetText(llButton *widget, uint8_t *text)
{
    uint32_t textLength;
    if(widget->isEnable)
    {
        textLength=strlen((const char*)text)+1;
        textLength=((textLength>LL_BUTTON_TEXT_LENGTH_MAX)?LL_BUTTON_TEXT_LENGTH_MAX:textLength);
        memcpy(widget->textInfo.text,text,textLength);
        //最后一个字节强制变0
        (widget->textInfo.text)[textLength-1]=0;
        pButtonRefresh(widget,false);
    }
}

void nButtonSetText(uint16_t nameId, uint8_t *text)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetText((llButton*)widget,text);
    }
}

uint8_t* pButtonGetText(llButton *widget)
{
    return widget->textInfo.text;
}

uint8_t* nButtonGetText(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetText((llButton*)widget);
    }
    return NULL;
}

void pButtonSetEnabled(llButton *widget, bool state)
{
    widget->isEnable=state;
    pButtonRefresh(widget,false);
}

void nButtonSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetEnabled((llButton*)widget,state);
    }
}

bool pButtonGetEnabled(llButton *widget)
{
    return widget->isEnable;
}

bool nButtonGetEnabled(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetEnabled((llButton*)widget);
    }
    return false;
}

void pButtonSetCheckable(llButton *widget, bool state)
{
    if(widget->isEnable)
    {
        widget->isCheckable=state;
    }
}

void nButtonSetCheckable(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetCheckable((llButton*)widget,state);
    }
}

bool pButtonGetCheckable(llButton *widget)
{
    return widget->isCheckable;
}

bool nButtonGetCheckable(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetCheckable((llButton*)widget);
    }
    return false;
}

void pButtonClick(llButton *widget)
{
    if(widget->isEnable)
    {
        llEmitSignal(widget,SIGNAL_CLICK_PRESSED);
        llEmitSignal(widget,SIGNAL_CLICK_RELEASED);
    }
}

void nButtonClick(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonClick((llButton*)widget);
    }
}

void pButtonSetChecked(llButton *widget, bool state)
{
    if((widget->isChecked!=state)&&(widget->isEnable))
    {
        pButtonClick(widget);
    }
}

void nButtonSetChecked(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        if(((llButton*)widget)->isCheckable==true)
        {
            if(((llButton*)widget)->isChecked!=state)
            {
                pButtonClick((llButton*)widget);
            }
        }
    }
}

bool pButtonGetChecked(llButton *widget)
{
    return widget->isChecked;
}

bool nButtonGetChecked(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetChecked((llButton*)widget);
    }
    return false;
}

bool pButtonGetPressed(llButton *widget)
{
    return widget->isPressed;
}

bool nButtonGetPressed(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetPressed((llButton*)widget);
    }
    return false;
}

void nButtonToggle(uint16_t nameId)
{
    nButtonClick(nameId);
}

void pButtonSetPreselected(llButton *widget, bool state)
{
    if(widget->isEnable)
    {
        widget->isSelected=state;
        pButtonRefresh(widget,false);
    }
}

void nButtonSetPreselected(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetPreselected((llButton*)widget,state);
    }
}

bool pButtonGetPreselected(llButton *widget)
{
    return widget->isSelected;
}

bool nButtonGetPreselected(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetPreselected((llButton*)widget);
    }
    return false;
}

void nButtonRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonRefresh((llButton*)widget,false);
    }
}

void pButtonSetKeyValue(llButton *widget,uint8_t value)
{
    if(widget->isEnable)
    {
        widget->keyValue=value;
    }
}

void nButtonSetKeyValue(uint16_t nameId, uint8_t value)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetKeyValue(widget,value);
    }
}

uint8_t pButtonGetKeyValue(llButton *widget)
{
    return widget->keyValue;
}

uint8_t nButtonGetKeyValue(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        return pButtonGetKeyValue(widget);
    }
    return 0;
}


void pButtonSetHidden(llButton *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pButtonRefresh(widget,false);
    }
}

void nButtonSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetHidden(widget,isHidden);
    }
}

void pButtonSetBuzzerCfg(llButton *widget,uint8_t cfg)
{
    if(widget->isEnable)
    {
        widget->buzzerCfg=cfg;
    }
}

void nButtonSetBuzzerCfg(uint16_t nameId,uint8_t cfg)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonSetBuzzerCfg(widget,cfg);
    }
}

void pButtonMove(llButton *widget,int16_t x, int16_t y)
{
    llGeometry oldGeometry,newGeometry;
    oldGeometry=widget->geometry;
    widget->isHidden=false;
    widget->geometry.x=x;
    widget->geometry.y=y;
    newGeometry=widget->geometry;
    llGeneralWidgetParentRecoverMove((llGeneral*)widget,oldGeometry,newGeometry);
    pButtonRefresh(widget,false);
}

void nButtonMove(uint16_t nameId,int16_t x, int16_t y)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonMove(widget,x,y);
    }
}
