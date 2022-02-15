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

#include "LL_ComboBox.h"
#include "LL_Linked_List.h"

void pComboBoxFree(llComboBox *widget)
{
    uint8_t i;
    for(i=0; i<LL_COMBOBOX_ITEM_MAX; i++)
    {
        llFree(widget->textList[i]);
    }
    llFree(widget);
}

void nComboBoxDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llComboBox *widget;

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
                pComboBoxFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llComboBoxAction(void *widget,uint8_t touchSignal)
{
    if((((llComboBox*)widget)->isEnable)&&(((llComboBox*)widget)->isHidden==false))
    {
        if((touchSignal==SIGNAL_CLICK_PRESSED)||(touchSignal==SIGNAL_CLICK_RELEASED)||(touchSignal==SIGNAL_CLICK_HOLD_MOVE))
        {
            llEmitSignal(widget,touchSignal);
        }
    }
}

void pComboBoxRefresh(llComboBox *widget)
{
    llPoint globalPos,tempPos1,tempPos2;
    int16_t textHeight;
    llChar textInfo;
    uint8_t i,r,g,b;
    int32_t colorOffset;
    
    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);

            globalPos.x=globalPos.x+widget->geometry.x;
            globalPos.y=globalPos.y+widget->geometry.y;


            //显示外框
            llDrawLine(globalPos.x,globalPos.y,globalPos.x+widget->geometry.width-1,globalPos.y,RGB888(0xABACAD));
            llDrawLine(globalPos.x,globalPos.y,globalPos.x,globalPos.y+widget->itemHeight-1,RGB888(0xABACAD));
            llDrawLine(globalPos.x+widget->geometry.width-1,globalPos.y,globalPos.x+widget->geometry.width-1,globalPos.y+widget->itemHeight-1,RGB888(0xABACAD));
            llDrawLine(globalPos.x,globalPos.y+widget->itemHeight-1,globalPos.x+widget->geometry.width-1,globalPos.y+widget->itemHeight-1,RGB888(0xABACAD));
            //显示底色
            llFillSingleColor(globalPos.x+1,globalPos.y+1,globalPos.x+widget->geometry.width-2,globalPos.y+widget->itemHeight-2,widget->bgColor);
            //画箭头
            tempPos1.x=globalPos.x+widget->geometry.width-1-(widget->itemHeight>>2);
            tempPos1.y=globalPos.y+widget->itemHeight/3;
            tempPos2.x=globalPos.x+widget->geometry.width-1-(widget->itemHeight>>1);
            tempPos2.y=globalPos.y+widget->itemHeight/3*2;
            llDrawLine(tempPos1.x,tempPos1.y,tempPos2.x,tempPos2.y,0);
            tempPos1.x=tempPos2.x-(widget->itemHeight>>2);
            llDrawLine(tempPos1.x,tempPos1.y,tempPos2.x,tempPos2.y,0);

            llCharOpenLibrary(widget->fontLibInfo->libType,(uint8_t *)widget->fontLibInfo->name,widget->fontLibInfo->fontSize);
            textHeight=llFontGetHeightSize();

            textInfo.text=llMalloc(widget->itemLenMax);

            //更新文字坐标
            textInfo.geometry.x=globalPos.x+5;
            textInfo.geometry.y=globalPos.y;
            textInfo.geometry.width=widget->geometry.width-5-widget->itemHeight;
            textInfo.geometry.height=widget->itemHeight;
            textInfo.showGeometry=textInfo.geometry;
            textInfo.isPassword=false;
            textInfo.isTransparent=true;
            textInfo.hAlign=llAlignHLeft;
            textInfo.vAlign=llAlignVCenter;
            textInfo.charColor=widget->textColor;

            strcpy((char*)textInfo.text,(const char*)widget->textList[widget->selectNum]);

            llCharDisplay(&(textInfo));

            if(widget->geometry.height!=widget->itemHeight)
            {

                //显示底色
                llFillSingleColor(globalPos.x+1,globalPos.y+widget->itemHeight,globalPos.x+widget->geometry.width-1,globalPos.y+(widget->itemCount+1)*widget->itemHeight-2,widget->listBgColor);
                //显示外框
                llDrawLine(globalPos.x,globalPos.y+widget->itemHeight,globalPos.x,globalPos.y+(widget->itemCount+1)*widget->itemHeight-1,RGB888(0xABACAD));
                llDrawLine(globalPos.x+widget->geometry.width-1,globalPos.y+widget->itemHeight,globalPos.x+widget->geometry.width-1,globalPos.y+(widget->itemCount+1)*widget->itemHeight-1,RGB888(0xABACAD));
                llDrawLine(globalPos.x,globalPos.y+(widget->itemCount+1)*widget->itemHeight-1,globalPos.x+widget->geometry.width-1,globalPos.y+(widget->itemCount+1)*widget->itemHeight-1,RGB888(0xABACAD));

                //选中项底色
                llFillSingleColor(globalPos.x+1,globalPos.y+widget->itemHeight*(widget->selectNum+1),globalPos.x+widget->geometry.width-1,globalPos.y+widget->itemHeight*(widget->selectNum+1)+widget->itemHeight-1,widget->selectBgColor);

                for(i=0; i<LL_COMBOBOX_ITEM_MAX; i++)
                {
                    if(widget->textList[i]!=NULL)
                    {
                        memset(textInfo.text,0,widget->itemLenMax);

                        //更新文字坐标
                        textInfo.geometry.y=globalPos.y+(i+1)*widget->itemHeight;
                        textInfo.showGeometry.y=textInfo.geometry.y;


                        strcpy((char*)textInfo.text,(const char*)widget->textList[i]);

                        textInfo.charColor=widget->textColor;
                        
                        if(i==widget->selectNum)
                        {
                            textInfo.charColor=RGB888(0xFFFFFF)-widget->textColor;
                        }
                        llCharDisplay(&(textInfo));
                    }
                    else
                    {
                        break;
                    }
                }
            }

            llFree(textInfo.text);
        }
    }
}

void nComboBoxRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeComboBox);
    if(widget!=NULL)
    {
        pComboBoxRefresh((llComboBox*)widget);
    }
}


bool slotComboBoxHoldMove(llConnectInfo info)
{
    llComboBox * widget;
    llGeometry targetGeometry;
    llPoint clickPoint;
    uint8_t i;

    widget=(llComboBox*)info.sender;



    if(widget->geometry.height!=widget->itemHeight)//(widget->isExpand==true)
    {
        targetGeometry=widget->geometry;
        targetGeometry.height=widget->itemHeight;

        if(llClickGetPoint(&clickPoint.x,&clickPoint.y))
        {
            for(i=0; i<widget->itemCount; i++)
            {
                targetGeometry.y+=widget->itemHeight;
                if((llPointInRect(clickPoint,targetGeometry))&&(widget->selectNum!=i))
                {
                    widget->selectNum=i;
                    pComboBoxRefresh(widget);
                    break;
                }
            }
        }
        else
        {
            widget->isExpand=false;
        }
    }
    return false;
}

bool slotComboBoxRelease(llConnectInfo info)
{
    llComboBox * widget;
    llGeometry targetGeometry;
    widget=(llComboBox*)info.sender;

    if((widget->isExpand==false)&&(widget->geometry.height!=widget->itemHeight))//选择新项目
    {
        targetGeometry=widget->geometry;
        targetGeometry.y+=widget->itemHeight;
        llGeneralRecover((llGeneral*)widget,targetGeometry);
        widget->geometry.height=widget->itemHeight;
        pComboBoxRefresh(widget);
    }
    return false;
}

bool slotComboBoxPress(llConnectInfo info)
{
    llComboBox * widget;
    llGeometry targetGeometry;
    llPoint clickPoint;
    uint8_t i;

    widget=(llComboBox*)info.sender;

    targetGeometry=widget->geometry;
    targetGeometry.height=widget->itemHeight;

    if(llClickGetPoint(&clickPoint.x,&clickPoint.y))
    {
        if(llPointInRect(clickPoint,targetGeometry))
        {
            if(widget->isExpand==false)
            {
                widget->isExpand=true;
                widget->geometry.height=widget->itemHeight*(widget->itemCount+1);
            }
            else
            {
                widget->isExpand=false;
                targetGeometry=widget->geometry;
                targetGeometry.y+=widget->itemHeight;
                llGeneralRecover((llGeneral*)widget,targetGeometry);
                widget->geometry.height=widget->itemHeight;
            }
            pComboBoxRefresh(widget);
        }
        else
        {
            //选择新项目
            for(i=0; i<widget->itemCount; i++)
            {
                targetGeometry.y+=widget->itemHeight;
                if(llPointInRect(clickPoint,targetGeometry))
                {
                    widget->selectNum=i;
                    llEmitSignal(widget,SIGNAL_VALUE_CHANGED);
                    break;
                }
            }
            pComboBoxRefresh(widget);
            widget->isExpand=false;
        }
    }
    else
    {
        widget->isExpand=false;
    }
    return false;
}

llComboBox* llComboBoxQuickCreate(uint16_t nameId, uint16_t parentNameId,int16_t x, int16_t y, int16_t width, int16_t height,
                                  llFontLib *fontLib,llColor textColor,llColor bgColor,llColor listBgColor,llColor selectBgColor,
                                  uint8_t defaultSelectNum,bool isHidden)
{
    llComboBox * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llComboBox);
        if(pNewWidget!=NULL)
        {
            pNewWidget->nameId=nameId;

            pNewWidget->widgetType=widgetTypeComboBox;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;
            pNewWidget->itemHeight=height;

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);
            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->deleteFunc=nComboBoxDelete;
            pNewWidget->actionFunc=llComboBoxAction;
            pNewWidget->refreshFunc=nComboBoxRefresh;

            pNewWidget->fontLibInfo=fontLib;
            pNewWidget->textColor=textColor;
            pNewWidget->bgColor=bgColor;
            pNewWidget->listBgColor=listBgColor;
            pNewWidget->selectBgColor=selectBgColor;
            pNewWidget->isExpand=false;
            pNewWidget->selectNum=defaultSelectNum;
            pNewWidget->itemLenMax=0;
            pNewWidget->itemCount=0;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;

            memset(pNewWidget->textList,NULL,LL_COMBOBOX_ITEM_MAX);

            //动作
            llConnectSignal(nameId,SIGNAL_CLICK_PRESSED,nameId,slotComboBoxPress);
            llConnectSignal(nameId,SIGNAL_CLICK_RELEASED,nameId,slotComboBoxRelease);
            llConnectSignal(nameId,SIGNAL_CLICK_HOLD_MOVE,nameId,slotComboBoxHoldMove);
        }
        else
        {
            llFree(pNewWidget);
        }
    }
    return pNewWidget;
}

llComboBox* llComboBoxCreate(uint16_t nameId, uint16_t parentNameId,int16_t x, int16_t y, int16_t width, int16_t height,
                             llFontLib *fontLib,bool isHidden)
{
    return llComboBoxQuickCreate(nameId,parentNameId,x,y,width,height,fontLib,0,RGB888(0xE1E1E1),RGB888(0xFFFFFF),RGB888(0x0078d7),0,false);
}

void pComboBoxAddItem(llComboBox *widget,uint8_t *itemText)
{
    uint8_t i;
    uint16_t textLen;

    if(widget->isEnable)
    {
        //查找空位
        for(i=0; i<LL_COMBOBOX_ITEM_MAX; i++)
        {
            if(widget->textList[i]==NULL)
            {
                break;
            }
        }
        if(widget->textList[i]==NULL)
        {
            textLen=strlen((const char*)itemText)+1;
            widget->textList[i]=llMalloc(textLen);
            memset(widget->textList[i],0,textLen);
            strcpy((char *)widget->textList[i],(const char *)itemText);

            if(widget->itemLenMax<textLen)
            {
                widget->itemLenMax=textLen;
            }
            widget->itemCount++;
        }
        pComboBoxRefresh(widget);
    }
}

void nComboBoxAddItem(uint16_t nameId,uint8_t *text)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeComboBox);
    if(widget!=NULL)
    {
        pComboBoxAddItem((llComboBox*)widget,text);
    }
}

//void pComboBoxSetHidden(llComboBox *widget,bool isHidden)
//{
//    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
//    {
//        pComboBoxRefresh(widget);
//    }
//}

//void nComboBoxSetHidden(uint16_t nameId,bool isHidden)
//{
//    void *widget;
//    widget=llGeneralGetWidget(nameId,widgetTypeComboBox);
//    if(widget!=NULL)
//    {
//        pComboBoxSetHidden(widget,isHidden);
//    }
//}

void pComboBoxSetEnabled(llComboBox *widget, bool state)
{
    widget->isEnable=state;
}

void nComboBoxSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeComboBox);
    if(widget!=NULL)
    {
        pComboBoxSetEnabled((llComboBox*)widget,state);
    }
}
