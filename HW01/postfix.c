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
#include <string.h>

/* 用來保存資料並代表Stack的Sturcture */
typedef struct mStack{
    int buffer[1024];
    int *sp;
    int count;
}mStack_t;

/* 一些Stack的基本操作 */
/* 第一個參數傳入的是要操作的Stack */
void stack_init(mStack_t *stack);
int stack_push(mStack_t *stack, int data);
int stack_pop(mStack_t *stack);
int stack_isEmpty(mStack_t *stack);

int main(void){
    char input_buffer[1024]; // String parsing用的Buffers
    char process_buffer[32];
    char *ptr; // 輸入掃描用
    int buf, err; // String轉int buffer, 錯誤指示

    /* 儲存數字用的 Stack */
    mStack_t operands;

    /* 初始化Stack */
    stack_init(&operands);

    printf("Please enter a postfix expression:\n"); //使用國小英語 詢問輸入

    /* 讀取輸入 */
    fgets(input_buffer, sizeof(input_buffer), stdin);
    ptr = input_buffer;

    err = 0;

    /* 掃描輸入, 根據讀取到的字元做操作 */
    do{
        sscanf(ptr, "%s", process_buffer);
        switch(process_buffer[0]){

            /* 四種運算子, 檢查是否有足夠的運算元後座運算 */
            case '+':
                if(operands.count < 2)
                    err = 1;
                stack_push(&operands, stack_pop(&operands) + stack_pop(&operands));
                break;
            case '-':
                if(operands.count < 2)
                    err = 1;
                stack_push(&operands, stack_pop(&operands) - stack_pop(&operands));
                break;
            case '*':
                if(operands.count < 2)
                    err = 1;
                stack_push(&operands, stack_pop(&operands) * stack_pop(&operands));
                break;
            case '/':
                if(operands.count < 2)
                    err = 1;
                stack_push(&operands, stack_pop(&operands) / stack_pop(&operands));
                break;

            /* 數字, 存入 Stack中 */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                sscanf(process_buffer, "%d", &buf);
                stack_push(&operands, buf);
                break;

            /* 其他, 如字母等, 視為錯誤 */
            default:
                err = 1;
                break;
        }
        
        /* 加ptr移動到下一個空白 */
        ptr += strlen(process_buffer) + 1;

    }while(*ptr != '\0');
    
    /* 檢查是否發生錯誤, 來決定輸出 */
    if(!err && operands.count == 1)
        printf("%d\n", stack_pop(&operands));
    else
        printf("It’s an illegal postfix expression.\n");

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
