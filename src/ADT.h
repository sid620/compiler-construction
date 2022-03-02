#pragma once

#include "ADTDef.h"

stack *initialize_stack();
void push(stack *stk, void* element);
void* pop(stack* stk);
void* get_top(stack *stk);
