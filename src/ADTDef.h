#pragma once

#define STACK_SIZE 100

typedef struct{
    void **stk;
    int current_size;
    int max_size;
} stack;
