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

#ifndef _LL_HANDLER_H_
#define _LL_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_Queue.h"

#define SYS_TIME_OUT_MS  500

//各种控件动作，鼠标动作，信号动作
#define SIGNAL_CLICK_NO_OPERATION    0 //无按下
#define SIGNAL_CLICK_PRESSED           1 //按下瞬间
#define SIGNAL_CLICK_HOLD_DOWN       2 //按下不释放
#define SIGNAL_CLICK_RELEASED          3 //释放瞬间
#define SIGNAL_CLICK_CHECK_PRESSED      4 // 单状态按下
#define SIGNAL_CLICK_CHECK_RELEASED      5 // 单状态释放
#define SIGNAL_CLICK_HOLD_MOVE      6 // 按下移动
#define SIGNAL_VALUE_CHANGED           7 //数据变化
#define SIGNAL_WIDGET_ACTIVE           8 //激活控件
#define SIGNAL_EIDTING_FINISHED           9 //编辑结束
#define SIGNAL_SYS_TIME_OUT             10//gui软件定时器

#define SIGNAL_USER                   100

typedef struct{
    void *sender;
    uint8_t senderSignalType;
}llEmitInfo;

typedef struct{
    void *sender;
    uint8_t senderSignalType;
    void *receiver;
}llConnectInfo;

//typedef struct{
//    uint16_t senderId;
//    uint8_t senderSignalType;
//    uint16_t receiverId;
//}llConnectExInfo;

typedef bool (*connectFunc)(llConnectInfo); //返回:忽略相同信号的其他操作

typedef struct{
    void *sender;
    uint8_t senderSignalType;
    void *receiver;
    connectFunc receiverFunc;
} llConnectRelation;

extern llQueue *emitQueue;

void llHandlerSetJumpPage(uint16_t targetPageNum, uint16_t backgroundNameId);
void llInit(void);
void llHandler(void);
bool llEmitSignal(void *widget,uint8_t touchSignal);
//void llDeleteEmitSignal(void *widget);
bool llConnectSignal(uint16_t senderId, uint8_t siganl, uint16_t receiverId, connectFunc func);
bool llDisconnectSignal(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func);
void llDeleteConnectSignal(uint16_t nameId);

#ifdef __cplusplus
}
#endif

#endif //_LL_HANDLER_H_
