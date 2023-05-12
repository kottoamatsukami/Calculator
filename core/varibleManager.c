#include "varibleManager.h"
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include <string.h>
#include "stack.h"
#include "operations.h"
#include "toPostfix.h"
#include "string.h"


int getVariableIndex(char* target, int num_of_vars, Variable pool[num_of_vars])
{
    for (int i = 0; i < num_of_vars; i++)
    {
        if (strcmp(target, pool[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

Complex FromTokenToComplex(Token* token, int num_of_vars, Variable pool[num_of_vars])
{
    Complex result;
    if (strcmp(token->type, "NUMBER")==0)
    {
        result.real = strtod(token->value, NULL);
        result.imag = 0;
        return result;
    }
    if (strcmp(token->type, "COMPLEX")==0)
    {
        result.real = 0;
        if (strlen(token->value) == 1)
        {
            result.imag = 1;
        }
        else
        {
            char buffer[MAX_TOKEN_VALUE_LEN];
            strcpy(token->value, buffer);
            buffer[strlen(buffer)-2] = '\0';
            result.imag = strtod(token->value, NULL);
        }
        return result;
    }
    if (strcmp(token->type, "VARIABLE")==0)
    {
        if (getVariableIndex(token->value, num_of_vars, pool) != -1)
        {
            result = pool[getVariableIndex(token->value, num_of_vars, pool)].expression.value;
            return result;
        }
        else
        {
            printf("Variable <%s> not found\n", token->value);
        }
    }
    return result;
}


void precalculate(int number_of_variables, Variable variable_pool[number_of_variables])
{
    for (int i = 0; i < number_of_variables; i++)
    {
        Stack reverse_rpn;
        Token last_token;
        InitializeToken(&last_token);
        InitializeStack(&reverse_rpn);
        int j = 0;
        while (
                // Закройте глаза, тут говнокодинг
                strcmp(variable_pool[i].expression.exp[j].type, "NUMBER") == 0 ||
                strcmp(variable_pool[i].expression.exp[j].type, "OPERATOR") == 0 ||
                strcmp(variable_pool[i].expression.exp[j].type, "FUNCTION") == 0)
        {
            // 1) Operator
            if (strcmp(variable_pool[i].expression.exp[j].type, "OPERATOR")==0)
            {
                Token first_token = PopStack(&reverse_rpn);
                Token second_token = PopStack(&reverse_rpn);
                Complex first_operand = FromTokenToComplex(&first_token, number_of_variables, variable_pool);
                Complex second_operand = FromTokenToComplex(&second_token, number_of_variables, variable_pool);
                PrintComplex(first_operand);
                //
//                Complex *result = malloc(sizeof(Complex));
//                result->real = first.value
//                Token stock = unary_operations()
//                PushStack(&reverse_rpn,)
            }
            // 2) Function
            if (strcmp(variable_pool[i].expression.exp[j].type, "FUNCTION")==0)
            {
                Token first = PopStack(&reverse_rpn);

            }

            PushStack(&reverse_rpn, &variable_pool[i].expression.exp[j]);
            j += 1;
        }

        // 3) Number, else -> error
        if (reverse_rpn.top == 1)
        {
            last_token = PopStack(&reverse_rpn);
            variable_pool[i].expression.value = FromTokenToComplex(&last_token, number_of_variables, variable_pool);
        }
    }
}

Complex* eval(Token *token_pool, int *num_of_vars, Variable pool[*num_of_vars])
{

    // get pull size
    int len_of_token_pool = 0;
    while (token_pool[len_of_token_pool].type[0] != '\0')
    {
        len_of_token_pool++;
    }

    /* Types of pool
     *  1) return variable value, example: >>> x1
     *  2) calculate expression, example: (145 + 24 * x), if x is noted, then return result
     *  3) assign expression, example: x = 5, just assign expression in x
     * */

    // 1 type
    if (len_of_token_pool == 1)
    {
        if (getVariableIndex(token_pool[0].value, *num_of_vars, pool) == -1)
        {
            printf("Variable have not been declared\n");
        }
        else
        {
            PrintComplex(FromTokenToComplex(&token_pool[0], *num_of_vars, pool));
        }
    }
    // 3 type
    else if (len_of_token_pool >= 2 && strcmp(token_pool[0].type, "VARIABLE") == 0 && strcmp(token_pool[1].value, "=") == 0)
    {
        if (getVariableIndex(token_pool[0].value, *num_of_vars, pool) == -1)
        {
            printf("New variable was created!\n");
            create_variable(token_pool[0].value, token_pool+2, num_of_vars, pool);
        }
        else
        {
            printf("Variable with the same name was created earlier, recreating\n");
            Variable *var = (Variable*)malloc(sizeof(Variable));
            var->name = token_pool[0].value;
            var->expression.exp = token_pool+2;
            pool[getVariableIndex(token_pool[0].value, *num_of_vars, pool)] = *var;
        }

    }
    // 2 type
    else
    {
        Stack reverse_rpn;
        Token last_token;
        InitializeToken(&last_token);
        InitializeStack(&reverse_rpn);
        int j = 0;
        while (
            // Закройте глаза, тут говнокодинг
                strcmp(token_pool[j].type, "NUMBER") == 0 ||
                strcmp(token_pool[j].type, "OPERATOR") == 0 ||
                strcmp(token_pool[j].type, "FUNCTION") == 0)
        {
            // 1) Operator
            if (strcmp(token_pool[j].type, "OPERATOR")==0)
            {
                printf("oper");
                Token first_token = PopStack(&reverse_rpn);
                Token second_token = PopStack(&reverse_rpn);
                Complex first_operand = FromTokenToComplex(&first_token, *num_of_vars, pool);
                Complex second_operand = FromTokenToComplex(&second_token, *num_of_vars, pool);
            }
            // 2) Function
            if (strcmp(token_pool[j].type, "FUNCTION")==0)
            {
                Token first = PopStack(&reverse_rpn);
            }

            PushStack(&reverse_rpn, &token_pool[j]);
            j += 1;
        }

        // 3) Number, else -> error
        if (reverse_rpn.top == 1)
        {
            last_token = PopStack(&reverse_rpn);
            PrintComplex(FromTokenToComplex(&last_token, *num_of_vars, pool));
        }
    }
}






Variable create_variable(char *name, Token *expression, int *num_of_vars, Variable pool[*num_of_vars])
{
    Variable *var = (Variable*)malloc(sizeof(Variable));
    var->name = name;
    var->expression.exp = expression;
    var->expression.is_calculated = 0;
    pool[*num_of_vars] = *var;
    (*num_of_vars)++;
    return *var;
}


void Print_Pool(int num_of_vars, Variable pool[num_of_vars])
{
    for (int i = 0; i < num_of_vars; i++)
    {
        printf("%s ", pool[i].name);
    }
    printf("\n");
}