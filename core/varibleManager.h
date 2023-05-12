#pragma once
#include "complex.h"
#include "token.h"

typedef struct {
    Token* exp;
//   char* requirement_vars;
    Complex value;
    int is_calculated;
} Expression;

typedef struct {
    char* name;
    Expression expression;
} Variable;

Variable create_variable(char *name, Token *expression, int *num_of_vars, Variable pool[*num_of_vars]);

Complex* eval(Token *token_pool, int *num_of_vars, Variable pool[*num_of_vars]);

void Print_Pool(int num_of_vars, Variable pool[num_of_vars]);

void precalculate(int number_of_variables, Variable variable_pool[number_of_variables]);