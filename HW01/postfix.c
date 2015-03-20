#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct mStack{
    int buffer[1024];
    int *sp;
    int count;
}mStack_t;

void stack_init(mStack_t *stack);
int stack_push(mStack_t *stack, int data);
int stack_pop(mStack_t *stack);
int stack_isEmpty(mStack_t *stack);

int main(void){
    char input_buffer[1024];
    char process_buffer[32];
    char *ptr;
    int buf, err;

    mStack_t operators;
    mStack_t operands;

    stack_init(&operators);
    stack_init(&operands);

    printf("Please entered a postfix expression:\n");

    fgets(input_buffer, sizeof(input_buffer), stdin);
    ptr = input_buffer;

    err = 0;

    do{
        sscanf(ptr, "%s", process_buffer);
        switch(process_buffer[0]){
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
            default:
                sscanf(process_buffer, "%d", &buf);
                stack_push(&operands, buf);
                break;
        }

        while(*ptr != ' ' && *ptr != '\0'){
            ptr++;
        }
        ptr++;

    }while(*ptr != '\0');
    
    if(!err && operands.count == 1)
        printf("%d\n", stack_pop(&operands));
    else
        printf("It’s an illegal postfix expression.\n");

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
