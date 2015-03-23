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

typedef struct mStack{
    int buffer[1024];
    int *sp;
    int count;
}mStack_t;

typedef struct mQueue{
    int buffer[1024];
    int *head;
    int *tail;
    int count;
}mQueue_t;

void stack_init(mStack_t *stack);
int stack_push(mStack_t *stack, int data);
int stack_pop(mStack_t *stack);
int stack_isEmpty(mStack_t *stack);

void queue_init(mQueue_t *queue);
int queue_push(mQueue_t *queue, int data);
int queue_pop(mQueue_t *queue);
int queue_isEmpty(mQueue_t *queue);

int data[32] = {70, 67, 75, 55, \
                51, 40, 87, 65, \
                58, 86, 66, 43, \
                57, 76, 68, 61, \
                60, 63, 39, 74
                };

int main(void){
    int i, guysOnBus;
    int totalWeight, buf;

    mStack_t bus;
    mQueue_t inBusStop;
    mQueue_t notInBusStop;
    mQueue_t destination;

    stack_init(&bus);
    queue_init(&inBusStop);
    queue_init(&notInBusStop);
    queue_init(&destination);

    for(i = 0; i < 20; i++){
        queue_push(&notInBusStop, data[i]);
    }

    for(i = 0; i < 4; i ++){
        buf = queue_pop(&notInBusStop);
        printf("A guy with %dkg enter the station\n", buf);
        queue_push(&inBusStop, buf);
    }
    while(destination.count < 20){
        for(i = 0; i < 4; i ++){
            buf = queue_pop(&notInBusStop);
            printf("A guy with %dkg enter the station\n", buf);
            queue_push(&inBusStop, buf);
        }

        totalWeight = 0;
        for(guysOnBus = 0; guysOnBus < 4;){
            buf = queue_pop(&inBusStop);
            totalWeight += buf;
            printf("A guy with %dkg try to get on the bus\n", buf);
            
            if(totalWeight > 260){
                totalWeight -= buf;
                queue_push(&inBusStop, buf);
                printf("The guy with %dkg is too fat. Thus, leaving the bus\n", buf);
            }else{
                stack_push(&bus, buf);
                printf("The guy with %dkg is on bus.\n", buf);
                guysOnBus++;
            }
        }

        for(i = 0; i < 4; i++){
            buf = stack_pop(&bus); 
            printf("A guy with %dkg arrived at destination\n", buf);
            queue_push(&destination, buf);
        }

        printf("===================================\n");
    }

    return 0;
}

void stack_init(mStack_t *stack){
    stack->sp = stack->buffer;
    stack->count = 0;

    return;
}

int stack_push(mStack_t *stack, int data){
    if(stack->count >= 1024){
        return 1;
    }
    *(++stack->sp) = data;
    stack->count++;

    return 0;
}

int stack_pop(mStack_t *stack){
    if(stack->count == 0){
        return 0;
    }
    stack->count--;

    return *(stack->sp--);
}

int stack_isEmpty(mStack_t *stack){
    return !(stack->count);
}

void queue_init(mQueue_t *queue){
    queue->tail = queue->head = queue->buffer;
    queue->count = 0;

    return;
}

int queue_push(mQueue_t *queue, int data){
    if(queue->count >= 1024){
        return 1;
    }
    *(queue->head++) = data;
    if(queue->head >= queue->buffer + 1024){
        queue->head = queue->buffer;
    }

    queue->count++;

    return 0;
}

int queue_pop(mQueue_t *queue){
    int ret;

    if(queue->count == 0){
        return 0;
    }
    queue->count--;

    ret =  *(queue->tail++);

    if(queue->tail >= queue->buffer + 1024){
        queue->tail = queue->buffer;
    }

    return ret;
}

int queue_isEmpty(mQueue_t *queue){
    return !(queue->count);
}
