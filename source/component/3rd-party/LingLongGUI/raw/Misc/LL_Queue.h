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

#ifndef _LL_QUEUE_H_
#define _LL_QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include "LL_General.h"

typedef struct
{
    void *pDataBase;
    uint32_t head;
    uint32_t tail;
    uint32_t maxsize;
}llQueue;


llQueue* llQueueCreate(uint32_t length, uint32_t itemSize);

bool llQueueEnqueue(llQueue *queue ,void * pInItem,uint32_t itemSize);

bool llQueueDequeue(llQueue *queue ,void * pOutItem,uint32_t itemSize);

uint32_t llQueueGetLength(llQueue *queue);

void llQueueClear(llQueue* queue);

#endif //_LL_QUEUE_H_
