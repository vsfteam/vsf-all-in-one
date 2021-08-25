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

#include "LL_Handler.h"
#include "LL_General.h"
#include "LL_Linked_List.h"
#include "LL_Config.h"
#include "LL_Image.h"
#include "LL_User.h"
#include "LL_Background.h"
#include "LL_Characters.h"
#include "LL_Timer.h"

llQueue *emitQueue;

static uint16_t jmupNewPage=0;
static uint16_t jumpBackgroundNameId=0;

static uint32_t llSysTimer=0;

void llInit(void)
{
    emitQueue=llQueueCreate(16,sizeof (llEmitInfo));
    
    llWidgetLink.next=&llWidgetLink;
    llWidgetLink.prev=&llWidgetLink;
    llConnectLink.next=&llConnectLink;
    llConnectLink.prev=&llConnectLink;
    
    llExFlashInit();
    
    llUserInit();
    
    llTimer_start(&llSysTimer);
}

bool llEmitSignal(void *widget,uint8_t touchSignal)
{
    llEmitInfo emitInfo;
    emitInfo.sender=widget;
    emitInfo.senderSignalType=touchSignal;
    return llQueueEnqueue(emitQueue,&emitInfo,sizeof (llEmitInfo));
}

//删除对应控件的所有信号
//void llDeleteEmitSignal(void *widget)
//{
//    llEmitInfo emitInfo;
//    bool isMatch;
//    //把所有信号读取出来,如果信号sender和控件不匹配,就重新放入队列中
//    while(llQueueGetLength(emitQueue)>0)
//    {
//        isMatch=false;
//        if(llQueueDequeue(emitQueue,&emitInfo,sizeof (llEmitInfo)))
//        {
//            if(emitInfo.sender==widget)
//            {
//                isMatch=true;
//            }
//        }
//        if(isMatch==false)
//        {
//            llQueueEnqueue(emitQueue,&emitInfo,sizeof (llEmitInfo));
//        }
//    }
//}

bool llConnectSignal(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func)
{
    llConnectRelation connectRelation;
    llListWidgetInfo *linkInfo;
    //根据名称查找widget

    if(llList_GetInfoByName(&linkInfo,senderId)==true)
    {
        connectRelation.sender=linkInfo->widget;
    }
    else
    {
        return false;
    }

    if(llList_GetInfoByName(&linkInfo,receiverId)==true)
    {
        connectRelation.receiver=linkInfo->widget;
    }
    else
    {
        return false;
    }

    connectRelation.senderSignalType=siganl;
    connectRelation.receiverFunc=func;

    //确定是否重复添加链接
    if(llListConnectFind(connectRelation)==false)
    {
        // 添加链表
        llListConnectAdd(connectRelation);
    }

    return true;
}

bool llDisconnectSignal(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func)
{
    llListHead *tempPos,*safePos;
    llListConnectInfo *linkInfo;
    llConnectRelation relationInfo;

    //此处轮训 连接表 所有数据
    list_for_each_prev_safe(tempPos,safePos, &llConnectLink)
    {
        linkInfo = list_entry(tempPos, llListConnectInfo, parent_link_pos);
        if(linkInfo!=NULL)
        {
            relationInfo=linkInfo->info;

            if((((llGeneral*)(relationInfo.sender))->nameId==senderId)&&
                    (relationInfo.senderSignalType==siganl)&&
                    (((llGeneral*)(relationInfo.receiver))->nameId==receiverId)&&
                    (relationInfo.receiverFunc==func))
            {
                llLinkedListDelete(tempPos);
                llFree(linkInfo);
                return true;
            }
        }
    }

    return false;
}

void llDeleteConnectSignal(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListConnectInfo *linkConnectTempInfo;
    llConnectRelation relation;

    //清除connect链表,应该要先核对connect链表上的sender，receiver和背景上的控件对应，才进行删除
    list_for_each_prev_safe(tempPos, safePos, &llConnectLink)
    {
        linkConnectTempInfo = list_entry(tempPos, llListConnectInfo, parent_link_pos);
        if(linkConnectTempInfo!=NULL)
        {
            relation=linkConnectTempInfo->info;

            if((((llGeneral*)relation.sender)->nameId==nameId)||(((llGeneral*)relation.receiver)->nameId==nameId))
            {
                llLinkedListDelete(tempPos);
                llFree(linkConnectTempInfo);
            }
        }
    }
}

static void *prevWidget;
void llHandlerClickedAction(uint8_t touchSignal,int16_t x,int16_t y)
{
    void *widget;

    static int16_t prevX,prevY;

    switch(touchSignal)
    {
    case SIGNAL_CLICK_NO_OPERATION:
    {
        break;
    }
    case SIGNAL_CLICK_PRESSED:
    {
        widget=llListGetWidget(x,y);
        prevX=x;
        prevY=y;
        prevWidget=widget;//准备数据,释放时候使用

        if(widget!=NULL)
        {
            ((llGeneral*)widget)->actionFunc(widget,touchSignal);
        }
        break;
    }
    case SIGNAL_CLICK_HOLD_DOWN:
    {
        if((prevX!=x)||(prevY!=y))
        {
            prevX=x;
            prevY=y;
            widget=prevWidget;//不可以把static变量作为函数变量调用
            if(widget!=NULL)
            {
                ((llGeneral*)widget)->actionFunc(widget,SIGNAL_CLICK_HOLD_MOVE);
            }
        }
        break;
    }
    case SIGNAL_CLICK_RELEASED:
    {
        widget=prevWidget;
        if(widget!=NULL)
        {
            ((llGeneral*)widget)->actionFunc(widget,touchSignal);
        }
        break;
    }
    default:
        break;
    }
}

void llHandlerClickedProcess(void)
{
    int16_t x;
    int16_t y;
    bool nowState;
    static bool prevState=TOUCH_NO_CLICK;
    uint8_t touchSignal=SIGNAL_CLICK_NO_OPERATION;

    /*==============================================================================
    get touch action and postion
    ==============================================================================*/

    nowState = llClickGetPoint(&x,&y);

    if(nowState==TOUCH_CLICK)
    {
        if(prevState==TOUCH_NO_CLICK)//按下,                下降沿触发
        {
            touchSignal=SIGNAL_CLICK_PRESSED;
        }
        else// prevState==TOUCH_CLICK 按住                低电平
        {
            touchSignal=SIGNAL_CLICK_HOLD_DOWN;
        }
    }
    else// nowState==TOUCH_NO_CLICK 无按下
    {
        if(prevState==TOUCH_NO_CLICK)//无按下                高电平
        {
            touchSignal=SIGNAL_CLICK_NO_OPERATION;
        }
        else// prevState==TOUCH_CLICK 按下,上升沿触发       上降沿触发
        {
            touchSignal=SIGNAL_CLICK_RELEASED;
        }
    }

    prevState=nowState;


    llHandlerClickedAction(touchSignal,x,y);
}

void llHandlerConnectProcess(void)
{
    llEmitInfo emitInfo;
//    bool ret;
    llListHead *temp_pos,*safePos;
    llListConnectInfo *linkInfo;
    llConnectRelation relationInfo;
    llConnectInfo connectInfo;
    bool ignoreSignal=false;//忽略相同信号的其他操作
    if(llQueueGetLength(emitQueue)>0)
    {
        if(llQueueDequeue(emitQueue,&emitInfo,sizeof (emitInfo)))
        {
            //此处轮训 连接表 所有数据
            list_for_each_prev_safe(temp_pos,safePos, &llConnectLink)
            {
                linkInfo = list_entry(temp_pos, llListConnectInfo, parent_link_pos);
                if(linkInfo!=NULL)
                {
                    relationInfo=linkInfo->info;

                    if((relationInfo.sender==emitInfo.sender)&&(relationInfo.senderSignalType==emitInfo.senderSignalType))
                    {
                        connectInfo.sender=relationInfo.sender;
                        connectInfo.senderSignalType=relationInfo.senderSignalType;
                        connectInfo.receiver=relationInfo.receiver;
                        ignoreSignal=relationInfo.receiverFunc(connectInfo);
                        
                        if(ignoreSignal==true)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}



void llHandlerSetJumpPage(uint16_t targetPageNum,uint16_t backgroundNameId)
{
    jumpBackgroundNameId=backgroundNameId;
    jmupNewPage=targetPageNum;
}

volatile bool isQuitPage=false;
void llHandlerGUIProcess(void)
{
    static uint16_t nowPage=0xffff;

    if(jmupNewPage!=nowPage)
    {
        if(jmupNewPage<userPageMax)
        {
            if(nowPage!=0xffff)
            {
                isQuitPage=true;
                llCharStopCursorBlink();
                pageQuitFunc[nowPage]();//退出页面前,先运行退出函数
                nBackgroundDelete(jumpBackgroundNameId);
                prevWidget=NULL;
                isQuitPage=false;
            }

            nowPage=jmupNewPage;
            pageInitFunc[nowPage]();
        }
    }
    else
    {
        pageLoopFunc[nowPage]();
    }

    llCharCursorBlinkLoop();

}



void llHandler(void)
{
    llListWidgetInfo *tempInfo;
    
    llHandlerClickedProcess();

    llHandlerConnectProcess();
    llHandlerGUIProcess();

    if( llTimer_timeOut(&llSysTimer,SYS_TIME_OUT_MS,true))
    {
        tempInfo = list_entry((&llWidgetLink)->next, llListWidgetInfo, parent_link_pos);

        llEmitSignal(tempInfo->widget,SIGNAL_SYS_TIME_OUT);
    }
    
    llDoubleBufferRefresh();
}


