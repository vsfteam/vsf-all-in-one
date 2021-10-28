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

#include "LL_Queue.h"
#include <string.h>


llQueue* llQueueCreate(uint32_t length, uint32_t itemSize)
{
    llQueue *pNewQueue;
    uint32_t  Queue_Data_Buff_Size;
    
    Queue_Data_Buff_Size=length*itemSize;
    //申请队列总空间
    pNewQueue=( llQueue * ) llMalloc ( sizeof( llQueue ) + Queue_Data_Buff_Size ) ;
    
    if(pNewQueue!=NULL) 
    {
    
        memset(pNewQueue,0,sizeof( llQueue ) + Queue_Data_Buff_Size);
        //分配内存成功
        pNewQueue->head=0;
        pNewQueue->tail=0;
        pNewQueue->maxsize=length;
//        pNewQueue->pDataBase=(void *)( (uint16_t)pNewQueue + (uint16_t)sizeof( llQueue )); //s12
        pNewQueue->pDataBase=(void *)( (uint32_t)pNewQueue + sizeof( llQueue )); //M3
    }
    return pNewQueue;
}

//void llQueueDelete(llQueue* queue)
//{
//    XFREE(queue);
//}

void llQueueClear(llQueue* queue)
{
    queue->head=queue->tail;
}

bool llQueueIsFull(llQueue *queue)
{
    if(queue->head==(queue->tail+1)%queue->maxsize)
        return true;
    else
        return false;
}

bool llQueueIsEmpty(llQueue *queue)
{
    if(queue->head==queue->tail)
        return true;
    else
    {
        return false;
    }
}

uint32_t llQueueGetLength(llQueue *queue)
{
    if(llQueueIsEmpty(queue)==true)
    {
        return 0;
    }
    else
    {
        if(llQueueIsFull(queue)==true)
        {
            return queue->maxsize-1;
        }
        else
        {
            if(queue->tail<queue->head)
            {
                return (queue->maxsize-queue->head)+queue->tail;
            }
            else
            {
                return queue->tail-queue->head;
            }
        }
    }
}

bool llQueueEnqueue(llQueue *queue ,void * pInItem,uint32_t itemSize)
{
    // 数据首地址 queue->pDataBase+uiItemSize*(queue->tail)
    
    //判断是否满
    if(llQueueIsFull(queue)==false)
    {
        memcpy((uint8_t*)queue->pDataBase+itemSize*(queue->tail),pInItem,itemSize);
        queue->tail=(queue->tail+1)%queue->maxsize;
        return true;
    }
    else
    {
        return false;
    }
}

bool llQueueDequeue(llQueue *queue ,void * pOutItem,uint32_t itemSize)
{
    //判断是否空
    if(llQueueIsEmpty(queue)==false)
    {        
        memcpy(pOutItem,(uint8_t*)queue->pDataBase+itemSize*(queue->head),itemSize);
        
        queue->head=(queue->head+1)%queue->maxsize;

        return true;
    }
    else
    {
        return false;
    }
}

