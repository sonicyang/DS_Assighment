/*========================================================================
 *  Copyright (c) 2015 sonicyang
 *
 *  This file is part of Bus.
 *
 *  Bus is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.

 *  Bus is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Bus.  If not, see <http://www.gnu.org/licenses/>.
=========================================================================*/



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* 用來保存資料並代表Stack的Sturcture */
typedef struct mStack{
    int buffer[1024];
    int *sp;
    int count;
}mStack_t;

/* 用來保存資料並代表Queue的Sturcture */
typedef struct mQueue{
    int buffer[1024];
    int *head;
    int *tail;
    int count;
}mQueue_t;

/* 一些Stack的基本操作 */
/* 第一個參數傳入的是要操作的Stack */
void stack_init(mStack_t *stack);
int stack_push(mStack_t *stack, int data);
int stack_pop(mStack_t *stack);
int stack_isEmpty(mStack_t *stack);

/* 一些Queue的基本操作 */
/* 第一個參數傳入的是要操作的Queue */
void queue_init(mQueue_t *queue);
int queue_push(mQueue_t *queue, int data);
int queue_pop(mQueue_t *queue);
int queue_isEmpty(mQueue_t *queue);

/* 測資 */
int data[32] = {70, 67, 75, 55, \
                51, 40, 87, 65, \
                58, 86, 66, 43, \
                57, 76, 68, 61, \
                60, 63, 39, 74
                };

int main(void){
    int i, guysOnBus; // 計數器
    int totalWeight, buf; // 目前車上重量, 暫存區

    mStack_t bus; //公車
    mQueue_t inBusStop; //車站
    mQueue_t notInBusStop; //未進入車站的人
    mQueue_t destination; //電機系館

    /* 初始化 */
    stack_init(&bus);
    queue_init(&inBusStop);
    queue_init(&notInBusStop);
    queue_init(&destination);

    /* 載入測資*/
    for(i = 0; i < 20; i++){
        queue_push(&notInBusStop, data[i]);
    }

    /* 先讓初使的4個人進站 */
    for(i = 0; i < 4; i ++){
        buf = queue_pop(&notInBusStop);
        printf("A guy with %dkg enter the station\n", buf);
        queue_push(&inBusStop, buf);
    }

    /* 再繫感有20個人以前 */
    while(destination.count < 20){

        
        /* 再讓4個人進站, 總共8個 */
        for(i = 0; i < 4; i ++){
            if(!queue_isEmpty(&notInBusStop)){
                buf = queue_pop(&notInBusStop);
                printf("A guy with %dkg enter the station\n", buf);
                queue_push(&inBusStop, buf);
            }
        }

        /* 開始跟具體重上車, 車上不到四人，持續嘗試 */
        totalWeight = 0;
        for(guysOnBus = 0; guysOnBus < 4;){

            /* 先上車 */
            buf = queue_pop(&inBusStop);
            totalWeight += buf;
            printf("A guy with %dkg try to get on the bus\n", buf);
            
            /* 檢查是否太胖 */
            if(totalWeight > 260){
                /* 歧視胖子, 把胖子趕下車, 排到隊伍後面*/
                totalWeight -= buf;
                queue_push(&inBusStop, buf);
                printf("The guy with %dkg is too fat. Thus, leaving the bus\n", buf);
            }else{
                /* 沒事, 已上車 */
                stack_push(&bus, buf);
                printf("The guy with %dkg is on bus.\n", buf);
                guysOnBus++;
            }
        }

        /* 到達系館, 下車 */
        for(i = 0; i < 4; i++){
            buf = stack_pop(&bus); 
            printf("A guy with %dkg arrived at destination\n", buf);
            queue_push(&destination, buf);
        }

        printf("===================================\n");
    }

    return 0;
}

/* Stack 初始化 */
void stack_init(mStack_t *stack){
    stack->sp = stack->buffer;
    stack->count = 0;

    return;
}

/* Stack Push */
int stack_push(mStack_t *stack, int data){

    /* 檢查是否會 Overflow */
    if(stack->count >= 1024){
        return 1;
    }

    /* 存入資料, 移動頂端位置 */
    *(++stack->sp) = data;
    stack->count++;

    return 0;
}

/* Stack Pop */
int stack_pop(mStack_t *stack){

    /* 檢查是否會 Underflow */
    if(stack->count == 0){
        return 0;
    }
    stack->count--;

    /* 讀取資料, 移動頂端位置 */
    return *(stack->sp--);
}

/* 檢查 Stack是否為空 */
int stack_isEmpty(mStack_t *stack){
    return !(stack->count);
}

/* Queue初始化 */
void queue_init(mQueue_t *queue){
    queue->tail = queue->head = queue->buffer;
    queue->count = 0;

    return;
}

/* Queue push */
int queue_push(mQueue_t *queue, int data){
    /* 檢查是否會 Overflow */
    if(queue->count >= 1024){
        return 1;
    }

    /* 存入資料, 移動前端位置, 使用RingBuffer實作 */
    *(queue->head++) = data;
    if(queue->head >= queue->buffer + 1024){
        queue->head = queue->buffer;
    }

    queue->count++;

    return 0;
}

/* Queue Pop */
int queue_pop(mQueue_t *queue){
    int ret;

    /* 檢查是否會 Underflow */
    if(queue->count == 0){
        return 0;
    }
    queue->count--;

    /* 讀取資料, 移動尾端位置, 使用RingBuffer實作 */
    ret =  *(queue->tail++);

    if(queue->tail >= queue->buffer + 1024){
        queue->tail = queue->buffer;
    }

    return ret;
}

/* 檢查 Queue 是否為空 */
int queue_isEmpty(mQueue_t *queue){
    return !(queue->count);
}
