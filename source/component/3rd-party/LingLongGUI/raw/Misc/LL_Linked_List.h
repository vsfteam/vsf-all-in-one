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

#ifndef _LL_LINKED_LIST_H_
#define _LL_LINKED_LIST_H_

#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"
#include "LL_General.h"
#include "LL_Handler.h"

struct list_head {
	struct list_head *next, *prev;
};

#define LL_LINKED_LIST(newLink)  struct list_head newLink={ &(newLink), &(newLink) }

static inline void llLinkedListAddTail(struct list_head *newList, struct list_head *head)
{
    struct list_head * prevObj;
    struct list_head * nextObj;

    //找出前后对象,中间插入,自身为下一个对象
    prevObj=head->prev;
    nextObj=head;
    
    //插入新对象
    nextObj->prev = newList;
    newList->next = nextObj;
    newList->prev = prevObj;
    prevObj->next= newList;
}

static inline void llLinkedListDelete(struct list_head *target)
{
    struct list_head * prevObj;
    struct list_head * nextObj;
    
    //找出前后对象
    prevObj=target->prev;
    nextObj=target->next;
    
    //前后指针相连
    nextObj->prev = prevObj;
	prevObj->next = nextObj;
}

#define list_entry(ptr, type, member)    (type *)((char*)(ptr) - offsetof(type, member))

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

typedef struct list_head llListHead;

typedef struct
{
    llListHead parent_link_pos;
    void *widget;
    llListHead child_link;
}llListWidgetInfo;

typedef struct
{
    llListHead parent_link_pos;
    llConnectRelation info;
    llListHead child_link;
}llListConnectInfo;

extern llListHead llWidgetLink;
extern llListHead llConnectLink;

void llListWidgetAdd(llListHead* parent_link, void* widget);
void llListConnectAdd(llConnectRelation info);
bool llListConnectFind(llConnectRelation info);
//uint8_t llListWidgetGetByPos(void** widget,int16_t x,int16_t y);
//uint8_t llListWidgetGetByName(void** widget, uint8_t *name);
bool llList_GetInfoByName(llListWidgetInfo **outInfo, uint16_t nameId);
//bool llList_GetParentInfoByName(llListWidgetInfo **outParentInfo, uint8_t *childName);
bool llListGetListByWidget(llListHead **outListPos, void* widget);
void* llListGetWidget(int16_t x,int16_t y);
void* llListGetRootWidget(void);
//bool llListIsWidgetInArea(void *widget,llGeometry geometry);
//void llListWidgetShowName(llListHead *in_link);
llPoint llListGetGlobalPos(llGeneral *widget);

#endif //_LL_LINKED_LIST_H_
