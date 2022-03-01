#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>

stack *initialize_stack(){
    stack *s = (stack *)malloc(sizeof(stack));
    s->max_size = STACK_SIZE;
    s->current_size = 0;
    s->stk = (void **)malloc(sizeof(void *) * (s->max_size));
    return s;
}

void push(stack *stk, void* element){
    if (stk->current_size == (stk->max_size)) {
        stk->max_size *= 2;
        void **s = realloc(stk->stk, sizeof(void *) * stk->max_size);
        stk->stk = s;
    }
    stk->stk[stk->current_size] = element;
    stk->current_size++;
}

void* get_top(stack *stk){
    if(stk->current_size == 0)  return NULL;
    else return stk->stk[stk->current_size-1];
}

void* pop(stack* stk){
    if (stk->current_size == 0) return NULL;
    stk->current_size--;
    return stk->stk[stk->current_size];
}
