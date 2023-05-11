#pragma once

#include "config.h"
#include "token.h"

typedef struct Stack {
    Token data[MAX_STACK_SIZE];
    int top;
} Stack;

void InitializeStack(Stack *stack);

int IsStackEmpty(Stack *stk);

void PushStack(Stack *stk, Token *tk);

Token PopStack(Stack *stk);

Token GetStackTop(Stack *stk);