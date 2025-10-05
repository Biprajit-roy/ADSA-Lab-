#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX 200

// Operator stack
char stack[MAX];
int top = -1;

// Evaluation stack (double for decimals)
double numStack[MAX];
int numTop = -1;

// Operator stack functions
void push(char c)
{
    if (top == MAX - 1)
    {
        printf("Operator Stack Overflow\n");
        return;
    }
    stack[++top] = c;
}

char pop()
{
    if (top == -1)
        return -1;
    return stack[top--];
}

char peek()
{
    if (top == -1)
        return -1;
    return stack[top];
}

// Number stack functions
void pushNum(double x)
{
    if (numTop == MAX - 1)
    {
        printf("Number Stack Overflow\n");
        return;
    }
    numStack[++numTop] = x;
}

double popNum()
{
    if (numTop == -1)
    {
        printf("Number Stack Underflow\n");
        return -1;
    }
    return numStack[numTop--];
}

// Operator precedence
int precedence(char c)
{
    if (c == '^')
        return 3;
    else if (c == '*' || c == '/')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

// Convert infix to postfix (handles multi-digit, negative, decimal)
void infixToPostfix(char *infix, char *postfix)
{
    int i = 0, k = 0;
    while (infix[i] != '\0')
    {
        char c = infix[i];

        // If number (digit or decimal point or negative sign in front of number)
        if (isdigit(c) || c == '.' ||
            ((c == '-' || c == '+') && (i == 0 || infix[i - 1] == '(')))
        {
            postfix[k++] = c;
            i++;
            while (isdigit(infix[i]) || infix[i] == '.')
            {
                postfix[k++] = infix[i++];
            }
            postfix[k++] = ' ';
            continue;
        }
        else if (c == '(')
        {
            push(c);
        }
        else if (c == ')')
        {
            while (top != -1 && peek() != '(')
            {
                postfix[k++] = pop();
                postfix[k++] = ' ';
            }
            pop(); // remove '('
        }
        else
        { // operator
            while (top != -1 && precedence(c) <= precedence(peek()))
            {
                postfix[k++] = pop();
                postfix[k++] = ' ';
            }
            push(c);
        }
        i++;
    }

    // Pop remaining operators
    while (top != -1)
    {
        postfix[k++] = pop();
        postfix[k++] = ' ';
    }
    postfix[k] = '\0';
}

// Evaluate postfix expression
double evaluatePostfix(char *postfix)
{
    int i = 0;
    while (postfix[i] != '\0')
    {
        if (isdigit(postfix[i]) || postfix[i] == '.' ||
            ((postfix[i] == '-' || postfix[i] == '+') && isdigit(postfix[i + 1])))
        {
            char *end;
            double num = strtod(&postfix[i], &end);
            pushNum(num);
            i = end - postfix; // move index forward
        }
        else if (postfix[i] == ' ')
        {
            // ignore spaces
            i++;
        }
        else
        { // operator
            double val2 = popNum();
            double val1 = popNum();
            switch (postfix[i])
            {
            case '+':
                pushNum(val1 + val2);
                break;
            case '-':
                pushNum(val1 - val2);
                break;
            case '*':
                pushNum(val1 * val2);
                break;
            case '/':
                pushNum(val1 / val2);
                break;
            case '^':
                pushNum(pow(val1, val2));
                break;
            }
            i++;
        }
    }
    return popNum();
}

// Main driver
int main()
{
    char infix[MAX], postfix[MAX];

    printf("Enter an infix expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = '\0'; // remove newline

    infixToPostfix(infix, postfix);
    printf("Postfix Expression: %s\n", postfix);

    double result = evaluatePostfix(postfix);
    printf("Evaluated Result: %.2f\n", result);

    return 0;
}
