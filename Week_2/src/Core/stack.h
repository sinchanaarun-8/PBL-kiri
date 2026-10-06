#ifndef STACK_H
#define STACK_H

#define STACK_CAPACITY 100

typedef struct
{
    int data[STACK_CAPACITY];
    int top;
} Stack;

void stack_init(Stack *stack);
void stack_reset(Stack *stack);

int stack_push(
    Stack *stack,
    int value
);

int stack_pop(
    Stack *stack,
    int *value
);

int stack_peek(
    const Stack *stack,
    int *value
);

#endif