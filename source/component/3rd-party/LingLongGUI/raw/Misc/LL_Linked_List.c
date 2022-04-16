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

#include "LL_Linked_List.h"
#include "LL_General.h"
#include "string.h"
#include "stdio.h"
#include "LL_Background.h"
#include "LL_Window.h"

LL_LINKED_LIST(llWidgetLink);
LL_LINKED_LIST(llConnectLink);

void llListWidgetAdd(llListHead* parent_link, void* widget)
{
    llListWidgetInfo * link = (llListWidgetInfo *)llMalloc(sizeof(llListWidgetInfo));
    if(link!=NULL)
    {
        link->widget=widget;

        llLinkedListAddTail(&(link->parent_link_pos), parent_link);
        link->child_link.next=&link->child_link;
        link->child_link.prev=&link->child_link;
    }
}


void llListConnectAdd(llConnectRelation info)
{
    llListConnectInfo *link=(llListConnectInfo *)llMalloc(sizeof(llListConnectInfo));
    if(link!=NULL)
    {

        link->info=info;
        llLinkedListAddTail(&(link->parent_link_pos), &llConnectLink);
        link->child_link.next=&link->child_link;
        link->child_link.prev=&link->child_link;
    }
}

//查找相同链接
bool llListConnectFind(llConnectRelation info)
{
    llListHead *temp_pos,*safePos;
    llListConnectInfo *link_info;
    llConnectRelation relationInfo;

    //判断链表是否为空
    if((llConnectLink.next!=&llConnectLink)&&(llConnectLink.prev!=&llConnectLink))
    {
        list_for_each_prev_safe(temp_pos,safePos, &llConnectLink)
        {
            link_info = list_entry(temp_pos, llListConnectInfo, parent_link_pos);
            if(link_info!=NULL)
            {
                relationInfo=link_info->info;

                if((relationInfo.sender==info.sender)&&
                   (relationInfo.senderSignalType==info.senderSignalType)&&
                   (relationInfo.receiver==info.receiver)&&
                   (relationInfo.receiverFunc==info.receiverFunc))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

/***************************************************************************//**
 * @fn         uint8_t llList_GetInfoByNameFromLink(llListHead in_link,llListWidgetInfo ** out_info,uint8_t *name)
 * @brief      根据唯一的名称(字符串),在指定控件链表中,查找链表节点数据
 * @param      in_link
 *             out_info
 *             *name
 * @return     uint8_t
 * @version    V0.1
 * @date       2018-12-08
 * @details    递归函数
 ******************************************************************************/
bool llList_GetInfoByNameFromLink(llListHead *in_link,llListWidgetInfo ** out_info,uint16_t nameId)
{
    llListHead *temp_pos,*safePos;
    llListWidgetInfo *link_info;
    llGeneral *temp_widget_info;

    //判断链表是否为空
    if((in_link->next!=in_link)&&(in_link->prev!=in_link))
    {
        list_for_each_prev_safe(temp_pos,safePos, in_link)
        {
            link_info = list_entry(temp_pos, llListWidgetInfo, parent_link_pos);
            if(link_info!=NULL)
            {
                temp_widget_info=(llGeneral*)(link_info->widget);
//                if(llStringCompare(nameId,temp_widget_info->nameId)==true)
                if(nameId==temp_widget_info->nameId)
                {
                    *out_info=link_info;
                    return true;
                }
                else
                {

                    if(llList_GetInfoByNameFromLink(&link_info->child_link,out_info,nameId)==true)
                    {
                        return true;
                    }

                }

            }
        }
    }
    return false;
}

/***************************************************************************//**
 * @fn         uint8_t llList_GetInfoByName(llListWidgetInfo **outInfo, uint8_t *name)
 * @brief      根据唯一的名称(字符串),在控件链表中,查找链表节点数据
 * @param      **outInfo
 *             *name
 * @return     uint8_t
 * @version    V0.1
 * @date       2018-12-08
 * @details
 ******************************************************************************/
bool llList_GetInfoByName(llListWidgetInfo **outInfo, uint16_t nameId)
{
    return llList_GetInfoByNameFromLink(&llWidgetLink,outInfo,nameId);
}

bool llListGetListByWidgetFromLink(llListHead *in_link,llListHead **outListPos,void* widget)
{
    llListHead *temp_pos,*safePos;
    llListWidgetInfo *link_info;

    //判断链表是否为空
    if((in_link->next!=in_link)&&(in_link->prev!=in_link))
    {
        list_for_each_prev_safe(temp_pos,safePos, in_link)
        {
            link_info = list_entry(temp_pos, llListWidgetInfo, parent_link_pos);
            if(link_info!=NULL)
            {
                if(widget==link_info->widget)
                {
                    *outListPos=temp_pos;
                    return true;
                }
                else
                {
                    if(llListGetListByWidgetFromLink(&link_info->child_link,outListPos,widget)==true)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool llListGetListByWidget(llListHead **outListPos, void* widget)
{
    return llListGetListByWidgetFromLink(&llWidgetLink,outListPos,widget);
}

void* llListGetWidgetFromLink(int16_t x,int16_t y,llListHead *inLink)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *tempInfo;
//    llGeneral *temp_widget_info;

    llPoint posT;
    int16_t widthT,heightT;
    void* ret;
    //继续查子控件
    list_for_each_prev_safe(tempPos,safePos, inLink)
    {
        tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);

            //获取全局坐标，用绝对值坐标进行比较
            posT= llListGetGlobalPos(((llGeneral*)tempInfo->widget));
            widthT=((llGeneral*)tempInfo->widget)->geometry.width;
            heightT=((llGeneral*)tempInfo->widget)->geometry.height;

        if(((llGeneral*)tempInfo->widget)->widgetType!=widgetTypeWindow)//不为窗体
        {
            if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT)))
            {
                return tempInfo->widget;//返回控件widget
            }
        }
        else//窗体
        {
            if(((llWindow*)tempInfo->widget)->isHidden==false)
            {
                //判断在窗体范围内
                if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT)))
                {
                ret=llListGetWidgetFromLink(x,y,&(tempInfo->child_link));
                if(ret!=NULL)
                {
                    return ret;
                }
                    else
                    {
                        return tempInfo->widget;
                    }
                }
            }
        }
    }

    return NULL;
}

void* llListGetWidget(int16_t x,int16_t y)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *tempInfo;

    //从后边开始查询  遍历控件
    list_for_each_prev_safe(tempPos,safePos, &llWidgetLink)
    {
        tempInfo = list_entry(tempPos,llListWidgetInfo, parent_link_pos);

        if(((llGeneral*)tempInfo->widget)->widgetType==widgetTypeBackground)//此处不能判断坐标范围
        {
            //继续查子控件
            return llListGetWidgetFromLink(x,y,&(tempInfo->child_link));
        }
    }
    return NULL;
}


void llListWidgetShowName(llListHead *in_link)
{
    llListHead *temp_pos,*safePos;
    llListWidgetInfo *link_info;

//    static uint8_t rootLevel=0;
    //判断链表是否为空
    if((in_link->next!=in_link)&&(in_link->prev!=in_link))
    {
        list_for_each_prev_safe(temp_pos,safePos, in_link)
        {
            link_info = list_entry(temp_pos, llListWidgetInfo, parent_link_pos);
            if(link_info!=NULL)
            {
                llListWidgetShowName(&link_info->child_link);
            }
        }
    }
}

//获取背景指针
void* llListGetRootWidget(void)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *tempInfo;

    //从后边开始查询  遍历控件
    list_for_each_prev_safe(tempPos,safePos, &llWidgetLink)
    {
        tempInfo = list_entry(tempPos,llListWidgetInfo, parent_link_pos);

        if(((llGeneral*)tempInfo->widget)->widgetType==widgetTypeBackground)//此处不能判断坐标范围
        {
            return tempInfo->widget;
        }
    }
    return NULL;
}

//获取全局坐标
llPoint llListGetGlobalPos(llGeneral *widget)
{
    llPoint pos,posParent;


    pos.x=widget->geometry.x;
    pos.y=widget->geometry.y;

    posParent.x=0;
    posParent.y=0;

    if(widget->parentType!=widgetTypeNone)
    {
        if(widget->parentType!=widgetTypeBackground)
        {
            posParent=llListGetGlobalPos(widget->parentWidget);
        }
        else
        {
            posParent.x=((llGeneral*)widget->parentWidget)->geometry.x;
            posParent.y=((llGeneral*)widget->parentWidget)->geometry.y;
        }
    }

    pos.x+=posParent.x;
    pos.y+=posParent.y;

    return pos;
}




