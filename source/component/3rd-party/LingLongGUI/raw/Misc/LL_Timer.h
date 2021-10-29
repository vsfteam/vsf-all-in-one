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

#ifndef _LL_TIMER_H_
#define _LL_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

// 放在定时器中断内
void llTimer_ticks(uint16_t cycleMs);

// 启动定时器
void llTimer_start(uint32_t *pTimer);

void llTimer_stop(uint32_t *pTimer);

/***************************************************************************//**
 * @fn         bool llTimer_timeOut(uint32_t *pTimer,uint32_t msec,bool isRestart);
 * @brief      超时检测
 * @param      *pTimer    计数变量
 *             msec       定时时间ms
 *             isRestart  超时触发后，是否自动重新启动
 * @return     bool       true:时间到 false:还没到设定时间
 * @version    V0.1
 * @date       2021-01-10
 * @details    
 ******************************************************************************/
bool llTimer_timeOut(uint32_t *pTimer,uint32_t msec,bool isRestart);

// 软件循环等待延时
void llTimer_delayMs(uint32_t delayMs);

/*

    uint32_t timerTest=0;

    llTimer_start(&timerTest);

    if(llTimer_timeOut(&timerTest,8000,true))
    {
        timer_beepOnce();
    }


*/













#endif //_LL_TIMER_H_






