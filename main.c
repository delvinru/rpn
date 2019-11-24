#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//Stack for Operations
struct stackNode {
    char operation;
    struct stackNode* nextPtr;
};

//Stack for Numbers
struct stackNumber {
    float res;
    struct stackNumber* nextNum;
};

typedef struct stackNode StackNode;
typedef StackNode* StackNodePtr;

typedef struct stackNumber StackNumber;
typedef StackNumber* StackNumberPtr;

//Prototype
void create_exp(char* exp, char* output, int n);
void calc_exp(char* exp);
int priority(char);
char pop_op(StackNodePtr* topPtr);
void push_op(StackNodePtr* topPtr, char oper);
float pop_num(StackNumberPtr* topPtr);
void push_num(StackNumberPtr* topPtr, float num);

int main(int argc, char** argv)
{
    if(argc == 1 || strcmp(argv[1], "-h") == 0  || strcmp(argv[1], "-help") == 0)
    {
        printf("Usage: ./polka '1+1='\nIt is important to use a sign at the end =\n");
        exit(0);
    }
    char* output;
    unsigned int n = strlen(argv[1]);
    printf("Expression: %s\n", argv[1]);
    output = (char*)malloc(n + 32);

    //Create RPN
    create_exp(argv[1], output, n);
    printf("Expressesion RPN: %s\n", output);

    //Calc RPN
    calc_exp(output);
    free(output);
    return 0;
}

char pop_op(StackNodePtr* topPtr)
{
    StackNodePtr tempPtr;

    tempPtr = *topPtr;
    char popValue = (*topPtr)->operation;
    *topPtr = (*topPtr)->nextPtr;
    free(tempPtr);
    return popValue;
}
void push_op(StackNodePtr* topPtr, char oper)
{
    StackNodePtr newPtr;
    newPtr = malloc(sizeof(StackNode));
    if (newPtr != NULL) {
        newPtr->operation = oper;
        newPtr->nextPtr = *topPtr;
        *topPtr = newPtr;
    }
    else
        printf("No memory avaible\n");
}
float pop_num(StackNumberPtr* topPtr)
{
    StackNumberPtr tempPtr;

    tempPtr = *topPtr;
    float popValue = (*topPtr)->res;
    *topPtr = (*topPtr)->nextNum;
    free(tempPtr);
    return popValue;
}
void push_num(StackNumberPtr* topPtr, float num)
{
    StackNumberPtr newPtr;
    newPtr = malloc(sizeof(StackNumber));
    if (newPtr != NULL) {
        newPtr->res = num;
        newPtr->nextNum = *topPtr;
        *topPtr = newPtr;
    }
    else
        printf("No memory avaible\n");
}

int priority(char sym)
{
    switch (sym) {
        case '(':
            return 0;
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            break;
    }
}

void create_exp(char* exp, char* output, int n)
{
    StackNodePtr stackPtr = NULL;
    int j = 0;
    char buffer[64];
    int counterNum = 0;
    for (int i = 0; i < n; i++) {
        if (isdigit(exp[i]))
            buffer[counterNum++] = exp[i];
        else if (counterNum != 0) {
            for (int k = 0; k < counterNum; k++)
                output[j++] = buffer[k];
            output[j++] = ' ';
            memset(buffer, 0, 64);
            counterNum = 0;
        }
        if (exp[i] == ' ') {
            output[j++] = exp[i];
            continue;
        }
        if (exp[i] == '(')
            push_op(&stackPtr, exp[i]);
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '^') {
            if (stackPtr != NULL && priority(stackPtr->operation) >= priority(exp[i])) {
                while (stackPtr != NULL && priority(stackPtr->operation) >= priority(exp[i])) {
                    output[j++] = pop_op(&stackPtr);
                    output[j++] = ' ';
                }
                push_op(&stackPtr, exp[i]);
            }
            else
                push_op(&stackPtr, exp[i]);
        }
        else if (exp[i] == ')') {
            char temp;
            while ((temp = pop_op(&stackPtr)) != '(') {
                output[j++] = temp;
                output[j++] = ' ';
            }
        }
    }
    while (stackPtr != NULL) {
        output[j++] = pop_op(&stackPtr);
        output[j++] = ' ';
    }
    output[j] = '\0';
}

void calc_exp(char* exp)
{
    StackNumberPtr stackNumPtr = NULL;
    char buffer[64];
    unsigned int j = 0;
    for (int i = 0; i < strlen(exp) - 1; i++) {
        if (isdigit(exp[i])) {
            buffer[j++] = exp[i];
            if (exp[i + 1] == ' ') {
                buffer[j] = '\0';
                push_num(&stackNumPtr, atof(buffer));
                memset(buffer, 0, 64);
                j = 0;
            }
            continue;
        }
        if (exp[i] == ' ')
            continue;
        if (stackNumPtr != NULL) {
            int num1 = pop_num(&stackNumPtr);
            int num2 = pop_num(&stackNumPtr);
            if (exp[i] == '+')
                push_num(&stackNumPtr, num2 + num1);
            else if (exp[i] == '-')
                push_num(&stackNumPtr, num2 - num1);
            else if (exp[i] == '/')
            {
                if(num1 == 0)
                {
                    printf("You divide by zero\n");
                    exit(0);
                } else
                    push_num(&stackNumPtr, num2/num1);
            }
            else if (exp[i] == '*')
                push_num(&stackNumPtr, num2 * num1);
            else if (exp[i] == '^')
                push_num(&stackNumPtr, (int)pow(num2, num1));
        }
    }
    printf("Result RPN: %.1f\n", stackNumPtr->res);
}