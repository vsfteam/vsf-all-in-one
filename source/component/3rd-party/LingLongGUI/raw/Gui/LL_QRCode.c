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

#include "LL_QRCode.h"
#include "LL_Linked_List.h"
#include "stdio.h"
#include "LL_Config.h"
#include "qrcodegen.h"

void llQRCodeAction(void *widget,uint8_t touchSignal)
{
    if(((llQRCode*)widget)->isEnable)
    {
        if((touchSignal==SIGNAL_CLICK_PRESSED)||(touchSignal==SIGNAL_CLICK_RELEASED))
        {
            llEmitSignal(widget,touchSignal);
        }
    }
}

void pQRCodeFree(llQRCode *widget)
{
    llFree(widget->qrText);
    llFree(widget);
}

void nQRCodeDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llQRCode *widget;
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
                pQRCodeFree(widget);
                llFree(tempInfo);
            }
        }
    }
}



void pQRCodeRefresh(llQRCode *widget)
{
    uint8_t *qr0;
    uint8_t *tempBuffer;
    uint32_t qrSize;
    bool ret;
    llColor tempColor;
    llPoint globalPos;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
        qr0=llMalloc(qrcodegen_BUFFER_LEN_FOR_VERSION(widget->qrMaxVersion));
        tempBuffer=llMalloc(qrcodegen_BUFFER_LEN_FOR_VERSION(widget->qrMaxVersion));

        if((qr0!=NULL)&&(tempBuffer!=NULL))
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);
            
            ret = qrcodegen_encodeText((const char*)widget->qrText,tempBuffer, qr0, widget->qrEcc,widget->qrMaxVersion, widget->qrMaxVersion,widget->qrMask,true);

            if (ret)
            {
                qrSize = qrcodegen_getSize(qr0);
                
                for (int y = 0; y < qrSize; y++)
                {
                    for (int x = 0; x < qrSize; x++)
                    {
                        if(qrcodegen_getModule(qr0, x, y))
                        {
                            tempColor=widget->qrColor;
                        }
                        else
                        {
                            tempColor=widget->bgColor;
                        }
                        for (int i=0; i<widget->qrZoom; i++)
                        {
                            llFillSingleColor(globalPos.x+widget->geometry.x+x*widget->qrZoom,      globalPos.y+widget->geometry.y+y*widget->qrZoom+i,
                                              globalPos.x+widget->geometry.x+(x+1)*widget->qrZoom-1,globalPos.y+widget->geometry.y+y*widget->qrZoom+i,
                                              tempColor);
                        }
                    }
                }
            }
        }

        llFree(qr0);
        llFree(tempBuffer);
    }
    }

}

void nQRCodeRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeQRCode);
    if(widget!=NULL)
    {
        pQRCodeRefresh((llQRCode*)widget);
    }
}

llQRCode *llQRCodeQuickCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x,uint16_t y,
                                uint8_t* qrText,llColor qrColor,llColor bgColor,
                                uint8_t qrEcc,uint8_t qrMask,uint8_t qrMaxVersion,uint8_t qrZoom,
                                bool isHidden)
{
    llQRCode * pNewWidget = NULL;
    uint8_t *pText = NULL;
    llListWidgetInfo *parentInfo;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llQRCode);
        pText=LL_MALLOC_STRING(qrText);
        if((pNewWidget!=NULL)&&(pText!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->widgetType=widgetTypeQRCode;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=0;
            pNewWidget->geometry.height=0;

            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);
            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->deleteFunc=nQRCodeDelete;
            pNewWidget->actionFunc=llQRCodeAction;
            pNewWidget->refreshFunc=nQRCodeRefresh;

            pNewWidget->qrText=pText;
            pNewWidget->qrEcc=qrEcc;
            pNewWidget->qrMask=qrMask;
            pNewWidget->qrMaxVersion=qrMaxVersion;
            pNewWidget->qrZoom=qrZoom;
            pNewWidget->qrColor=qrColor;
            pNewWidget->bgColor=bgColor;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;

            pQRCodeRefresh(pNewWidget);
        }
        else
        {
            llFree(pText);
            llFree(pNewWidget);
        }
    }
    return pNewWidget;
}

llQRCode *llQRCodeCreate(uint16_t nameId, uint16_t parentNameId, uint16_t x,uint16_t y,uint8_t* qrText,uint8_t qrEcc,uint8_t qrMask,uint8_t qrMaxVersion,uint8_t qrZoom,bool isHidden)
{
    return llQRCodeQuickCreate(nameId, parentNameId, x,y,
                                qrText,0,RGB888(0xffffff),
                                qrEcc,qrMask,qrMaxVersion,qrZoom,
                                isHidden);
}


void pQRCodeSetEnabled(llQRCode *widget, bool state)
{
    widget->isEnable=state;
}

void nQRCodeSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeQRCode);
    if(widget!=NULL)
    {
        pQRCodeSetEnabled((llQRCode*)widget,state);
    }
}

void pQRCodeSetText(llQRCode *widget, uint8_t *text)
{
    uint8_t *pText;
    if(widget->isEnable)
    {
        llFree(widget->qrText);
        pText=LL_MALLOC_STRING(text);
        strcpy((char *)pText,(const char *)text);
        widget->qrText=pText;
        pQRCodeRefresh(widget);
    }
}

void nQRCodeSetText(uint16_t nameId, uint8_t *text)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeQRCode);
    if(widget!=NULL)
    {
        pQRCodeSetText((llQRCode*)widget,text);
    }
}

uint8_t* pQRCodeGetText(llQRCode *widget)
{
    return widget->qrText;
}

uint8_t* nQRCodeGetText(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeQRCode);
    if(widget!=NULL)
    {
        return pQRCodeGetText((llQRCode*)widget);
    }
    return 0;
}

void pQRCodeSetHidden(llQRCode *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pQRCodeRefresh(widget);
    }
}

void nQRCodeSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeQRCode);
    if(widget!=NULL)
    {
        pQRCodeSetHidden(widget,isHidden);
    }
}

