#include <stdio.h>

#include "stack.h"


void stack_init(Stack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    stack->top = -1;
}


void stack_reset(Stack *stack)
{
    stack_init(stack);
}


int stack_push(
    Stack *stack,
    int value)
{
    if (stack == NULL)
    {
        return 0;
    }

    if (stack->top >= STACK_CAPACITY - 1)
    {
        printf("Stack: stack is full\n");
        return 0;
    }

    stack->top++;

    stack->data[stack->top] = value;

    return 1;
}


int stack_pop(
    Stack *stack,
    int *value)
{
    if (stack == NULL || value == NULL)
    {
        return 0;
    }

    if (stack->top < 0)
    {
        printf("Stack: stack is empty\n");
        return 0;
    }

    *value = stack->data[stack->top];

    stack->top--;

    return 1;
}


int stack_peek(
    const Stack *stack,
    int *value)
{
    if (stack == NULL || value == NULL)
    {
        return 0;
    }

    if (stack->top < 0)
    {
        printf("Stack: stack is empty\n");
        return 0;
    }

    *value = stack->data[stack->top];

    return 1;
}