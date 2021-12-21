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

#include "LL_Slider.h"
#include "LL_Linked_List.h"

void pSliderRefresh(llSlider *widget)
{
    llPoint globalPos;
    llPoint pos;
    uint16_t offset;
    llGeometry bgGeometry,curGeometry,newCurGeometry,recoveryGeometry;

    if(widget->isHidden==false)
    {
        if(llGeneralParentLinkHidden((llGeneral*)widget)==false)
        {
            globalPos=llListGetGlobalPos(widget->parentWidget);

            //计算slider坐标
            pos.x=globalPos.x+widget->geometry.x;
            pos.y=globalPos.y+widget->geometry.y;
            offset=widget->movePos;

            if(widget->isHorizontal==true)//水平
            {
                if(widget->isColor)
                {
                    //显示左侧
                    llFillSingleColor(pos.x,pos.y,pos.x+offset-1,pos.y+widget->geometry.height-1,widget->foregroundColor);
                    //显示右侧
                    llFillSingleColor(pos.x+offset+widget->cursorWidth,pos.y,pos.x+widget->geometry.width-1,pos.y+widget->geometry.height-1,widget->backgroundColor);
                    //显示游标
                    llFillSingleColor(pos.x+offset,pos.y,pos.x+offset+widget->cursorWidth-1,pos.y+widget->geometry.height-1,widget->cursorColor);
                }
                else
                {
                    //恢复背景
                    bgGeometry=widget->bgImgGeometry;
                    bgGeometry.x+=pos.x;
                    bgGeometry.y+=pos.y;
                    curGeometry=widget->curImgGeometry;
                    curGeometry.x+=pos.x;
                    curGeometry.y+=pos.y;
                    newCurGeometry=curGeometry;
                    newCurGeometry.x=pos.x+offset;

                            if(curGeometry.x<newCurGeometry.x)//恢复左侧
                            {
                                curGeometry.width=newCurGeometry.x-curGeometry.x;
                        llGeneralImageSpecificAreaShow((llGeneral*)widget,bgGeometry,curGeometry,widget->fgImgAddr);
                            }
                            else
                            {
                        if(curGeometry.x>newCurGeometry.x)//恢复右侧
                        {
                                curGeometry.width=curGeometry.x-newCurGeometry.x;
                                curGeometry.x=newCurGeometry.x+newCurGeometry.width;
                            llGeneralImageSpecificAreaShow((llGeneral*)widget,bgGeometry,curGeometry,widget->bgImgAddr);
                            }
                        }
                    //显示游标图片
                    llGeneralImageShow((llGeneral*)widget,widget->curImgAddr,pos.x+offset,pos.y);

                    widget->curImgGeometry.x=offset;
                }
            }
            else
            {
                if(widget->isColor)
                {
                    //显示上侧
                    llFillSingleColor(pos.x,pos.y,pos.x+widget->geometry.width-1,pos.y+offset-1,widget->backgroundColor);
                    //显示下侧
                    llFillSingleColor(pos.x,pos.y+offset,pos.x+widget->geometry.width-1,pos.y+widget->geometry.height-1,widget->foregroundColor);
                    //显示游标
                    llFillSingleColor(pos.x,pos.y+offset,pos.x+widget->geometry.width-1,pos.y+offset+widget->cursorWidth-1,widget->cursorColor);
                }
                else
                {
                    //恢复背景
                    bgGeometry=widget->bgImgGeometry;
                    bgGeometry.x+=pos.x;
                    bgGeometry.y+=pos.y;
                    curGeometry=widget->curImgGeometry;
                    curGeometry.x+=pos.x;
                    curGeometry.y+=pos.y;
                    newCurGeometry=curGeometry;
                    newCurGeometry.y=pos.y+offset;

                            if(curGeometry.y<newCurGeometry.y)//恢复上方
                            {
                                curGeometry.height=newCurGeometry.y-curGeometry.y;
                        llGeneralImageSpecificAreaShow((llGeneral*)widget,bgGeometry,curGeometry,widget->bgImgAddr);
                            }
                            else
                            {
                        if(curGeometry.y>newCurGeometry.y)//恢复下方
                        {
                                curGeometry.height=curGeometry.y-newCurGeometry.y;
                                curGeometry.y=newCurGeometry.y+newCurGeometry.height;
                            llGeneralImageSpecificAreaShow((llGeneral*)widget,bgGeometry,curGeometry,widget->fgImgAddr);
                            }
                        }
                    //显示游标图片
                    llGeneralImageShow((llGeneral*)widget,widget->curImgAddr,pos.x,pos.y+offset);
                    widget->curImgGeometry.y=offset;
                }
            }
        }
    }

}

void nSliderRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        pSliderRefresh((llSlider*)widget);
    }
}

void pSliderFree(llSlider *widget)
{
    llFree(widget);
}

void nSliderDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llSlider *widget;

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
                pSliderFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void llSliderAction(void *widget,uint8_t touchSignal)
{
    if(((llSlider*)widget)->isEnable)
    {
        switch(touchSignal)
        {
        case SIGNAL_CLICK_HOLD_MOVE:
        {
            llEmitSignal(widget,touchSignal);
            break;
        }
        default:
            break;
        }
    }
}

bool slotSliderMove(llConnectInfo info)
{
    llSlider * widget;
    int16_t x,y;
    int16_t offset;
    bool clickState;
    uint16_t maxLength=0;
    uint32_t u32Temp;

    widget=(llSlider*)info.receiver;

    clickState=llClickGetPoint(&x,&y);
    if(clickState==true)
    {
        if(widget->isHorizontal==true)//水平
        {
            //计算游标偏移值
            offset=x-widget->geometry.x;

            if(offset<0)
            {
                offset=0;
                //            widget->movePos=0;
            }
            else
            {
                if(offset>(widget->geometry.width-widget->cursorWidth))
                {
                    offset=widget->geometry.width-widget->cursorWidth;
                }
//                else
//                {
//                    if(!(((offset-widget->movePos)>3)||((offset-widget->movePos)<-3)))
//                    {
//                        offset=widget->movePos;
//                    }
//                }
            }

        }
        else
        {
            //计算游标偏移值
            offset=y-widget->geometry.y;

            if(offset<0)
            {
                offset=0;
                //            widget->movePos=0;
            }
            else
            {
                if(offset>(widget->geometry.height-widget->cursorWidth))
                {
                    offset=widget->geometry.height-widget->cursorWidth;
                }
//                else
//                {
//                    if(!(((offset-widget->movePos)>3)||((offset-widget->movePos)<-3)))
//                    {
//                        offset=widget->movePos;
//                    }
//                }
            }
        }

        if(widget->movePos!=offset)
        {
            widget->movePos=offset;
            if(widget->isAutoMove)
            {
                u32Temp=widget->movePos*100;
                if(widget->isHorizontal==true)//水平
                {
                    maxLength=widget->geometry.width-widget->cursorWidth;
                    widget->percent=u32Temp/maxLength;
                }
                else
                {
                    maxLength=widget->geometry.height-widget->cursorWidth;
                    widget->percent=u32Temp/maxLength;
                }
                pSliderRefresh(widget);
            }

            llEmitSignal(widget,SIGNAL_VALUE_CHANGED);
        }
    }
    return false;
}

llSlider *llSliderQuickCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,
                              llColor bgColor,llColor fgColor,llColor cursorColor,
                              uint32_t bgImgAddr,uint32_t fgImgAddr,uint32_t cursorImgAddr,
                              bool isColor,bool isHorizontal,bool isHidden)
{
    llSlider * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;
    llSize curSize,bgSize,fgSize;
    llPoint globalPos;
    llPoint pos;

    bool isRecvBufOk=true;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
    pNewWidget = LL_MALLOC_WIDGET_INFO(llSlider);
    if(!isColor)
    {
        isRecvBufOk=false;
    }
        if(pNewWidget!=NULL)
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeSlider;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;
            pNewWidget->isAutoMove=true;
            pNewWidget->percent=0;
            pNewWidget->isColor=isColor;

            pNewWidget->backgroundColor=bgColor;
            pNewWidget->foregroundColor=fgColor;
            pNewWidget->cursorColor=cursorColor;
            pNewWidget->cursorWidth=12;

            pNewWidget->bgImgAddr=bgImgAddr;
            pNewWidget->fgImgAddr=fgImgAddr;
            pNewWidget->curImgAddr=cursorImgAddr;

            if(!isColor)
            {
                globalPos=llListGetGlobalPos(pNewWidget->parentWidget);
                //计算slider坐标
                pos.x=globalPos.x+pNewWidget->geometry.x;
                pos.y=globalPos.y+pNewWidget->geometry.y;
                bgSize=llGeneralGetImageSize(bgImgAddr);
                fgSize=llGeneralGetImageSize(fgImgAddr);
                curSize=llGeneralGetImageSize(cursorImgAddr);
                if(isHorizontal)
                {
                    //自动更新控件尺寸
                    pNewWidget->geometry.width=bgSize.width;
                    //背景必须大于游标
                    pNewWidget->geometry.height=MAX(pNewWidget->geometry.height,bgSize.height);

                    //相对本控件的坐标
                    pNewWidget->bgImgGeometry.x=0;
                    pNewWidget->bgImgGeometry.y=RECTANGLE_MID_Y_POS(0,bgSize.height,bgSize.height);
                    pNewWidget->bgImgGeometry.width=bgSize.width;
                    pNewWidget->bgImgGeometry.height=bgSize.height;

                    pNewWidget->fgImgGeometry.x=0;
                    pNewWidget->fgImgGeometry.y=RECTANGLE_MID_Y_POS(0,fgSize.height,fgSize.height);
                    pNewWidget->fgImgGeometry.width=fgSize.width;
                    pNewWidget->fgImgGeometry.height=fgSize.height;

                    pNewWidget->curImgGeometry.x=0;
                    pNewWidget->curImgGeometry.y=RECTANGLE_MID_Y_POS(0,curSize.height,curSize.height);
                    pNewWidget->curImgGeometry.width=curSize.width;
                    pNewWidget->curImgGeometry.height=curSize.height;

                    pNewWidget->cursorWidth=curSize.width;

                    //显示背景图片
                    llGeneralImageShow((llGeneral*)pNewWidget,bgImgAddr,pNewWidget->bgImgGeometry.x+pos.x,pNewWidget->bgImgGeometry.y+pos.y);
                }
                else
                {
                    //自动更新控件尺寸
                    pNewWidget->geometry.height=bgSize.height;
                    //背景必须大于游标
                    pNewWidget->geometry.width=MAX(pNewWidget->geometry.width,bgSize.width);

                    //相对本控件的坐标
                    pNewWidget->bgImgGeometry.x=RECTANGLE_MID_X_POS(0,bgSize.width,bgSize.width);
                    pNewWidget->bgImgGeometry.y=0;
                    pNewWidget->bgImgGeometry.width=bgSize.width;
                    pNewWidget->bgImgGeometry.height=bgSize.height;

                    pNewWidget->fgImgGeometry.x=RECTANGLE_MID_X_POS(0,fgSize.width,fgSize.width);
                    pNewWidget->fgImgGeometry.y=0;
                    pNewWidget->fgImgGeometry.width=fgSize.width;
                    pNewWidget->fgImgGeometry.height=fgSize.height;

                    pNewWidget->curImgGeometry.x=RECTANGLE_MID_Y_POS(0,curSize.width,curSize.width);
                    pNewWidget->curImgGeometry.y=0;
                    pNewWidget->curImgGeometry.width=curSize.width;
                    pNewWidget->curImgGeometry.height=curSize.height;

                    pNewWidget->cursorWidth=curSize.height;

                    //显示背景图片
                    llGeneralImageShow((llGeneral*)pNewWidget,bgImgAddr,pNewWidget->bgImgGeometry.x+pos.x,pNewWidget->bgImgGeometry.y+pos.y);
                }

            }

            if(isHorizontal==true)
            {
                pNewWidget->movePos=0;
            }
            else
            {
                pNewWidget->movePos=height-pNewWidget->cursorWidth;
            }

            pNewWidget->isHorizontal=isHorizontal;
            //add linked list
            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);
            pNewWidget->deleteFunc=nSliderDelete;
            pNewWidget->actionFunc=llSliderAction;
            pNewWidget->refreshFunc=nSliderRefresh;
            pSliderRefresh(pNewWidget);

            //动作
            llConnectSignal(nameId,SIGNAL_CLICK_HOLD_MOVE,nameId,slotSliderMove);
        }
        else
        {
            llFree(pNewWidget);
        }
    }



    return pNewWidget;
}

llSlider *llSliderCreate(uint16_t nameId,uint16_t parentNameId,int16_t x, int16_t y,int16_t width, int16_t height,bool isHorizontal,bool isHidden)
{
    return llSliderQuickCreate(nameId,parentNameId,x,y,width,height,RGB888(0xBCBCBC),RGB888(0x2E90DD),RGB888(0xFFFFFF),0,0,0,true,isHorizontal,isHidden);
}

uint8_t pSliderGetClickPercent(llSlider* widget)
{
    uint16_t maxLength=0;
    uint32_t ret;
    maxLength=widget->geometry.width-widget->cursorWidth;

    ret=widget->movePos*100;
    return (ret/maxLength);
}

uint8_t nSliderGetClickPercent(uint16_t nameId)
{
    llSlider* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        return pSliderGetClickPercent(widget);
    }
    else
    {
        return 0;
    }
}

uint8_t pSliderGetPercent(llSlider* widget)
{
    if(widget->isHorizontal==true)//水平
    {
        return widget->percent;
    }
    else
    {
        return 100-widget->percent;
    }

//    uint16_t maxLength=0;
//    uint32_t ret;
//    maxLength=widget->geometry.width-widget->cursorWidth;
//
//    ret=widget->movePos*100;
//    return (ret/maxLength);
}

uint8_t nSliderGetPercent(uint16_t nameId)
{
    llSlider* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        return pSliderGetPercent(widget);
    }
    else
    {
        return 0;
    }
}

void pSliderSetPercent(llSlider* widget,uint8_t percent)
{
    float fTemp=0;

    if(widget->isEnable)
    {
        if(percent>100)
        {
            percent=100;
        }

        if(percent!=widget->percent)
        {
            widget->percent=percent;
            fTemp=(widget->geometry.width-widget->cursorWidth)/100.0;
            fTemp=fTemp*percent;
            widget->movePos=fTemp;
            if(widget->movePos>(widget->geometry.width+widget->geometry.x-widget->cursorWidth))
            {
                widget->movePos=widget->geometry.width+widget->geometry.x-widget->cursorWidth;
            }
            pSliderRefresh(widget);

            llEmitSignal(widget,SIGNAL_VALUE_CHANGED);
        }
    }
}

void nSliderSetPercent(uint16_t nameId,uint8_t percent)
{
    llSlider* widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        pSliderSetPercent(widget,percent);
    }
}


void pSliderSetEnabled(llSlider *widget, bool state)
{
    widget->isEnable=state;
}

void nSliderSetEnabled(uint16_t nameId, bool state)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        pSliderSetEnabled((llSlider*)widget,state);
    }
}

void pSliderSetHidden(llSlider *widget,bool isHidden)
{
    if(llGeometryAutoSetHiddenIsShow((llGeneral*)widget,isHidden)==true)
    {
        pSliderRefresh(widget);
    }
}

void nSliderSetHidden(uint16_t nameId,bool isHidden)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        pSliderSetHidden(widget,isHidden);
    }
}

void pSliderSetAutoMove(llSlider *widget,bool isAutoMove)
{
    if(widget->isEnable)
    {
        widget->isAutoMove=isAutoMove;
    }
}

void nSliderSetAutoMove(uint16_t nameId,bool isAutoMove)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeSlider);
    if(widget!=NULL)
    {
        pSliderSetAutoMove(widget,isAutoMove);
    }
}
