// C program for array implementation of stack
#include "stack.h"

const Point placeholder = {-1, -1};

// function to create a stack of given capacity. It initializes size of
// stack as 0
Stack* createStack(unsigned capacity)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (STACK_ITEM*)malloc(stack->capacity * sizeof(STACK_ITEM));
    return stack;
}
  
// Stack is full when top is equal to the last index
int Full(Stack* stack)
{
    return stack->top == stack->capacity - 1;
}
  
// Stack is empty when top is equal to -1
int Empty(Stack* stack)
{
    return stack->top == -1;
}
  
// Function to add an item to stack.  It increases top by 1
void push(Stack* stack, STACK_ITEM item)
{
    if (Full(stack))
        return;
    stack->array[++stack->top] = item;
    // printf("%d pushed to stack\n", item);
}
  
// Function to remove an item from stack.  It decreases top by 1
STACK_ITEM pop(Stack* stack)
{
    if (Empty(stack))
        return placeholder;
    return stack->array[stack->top--];
}
  
// Function to return the top from stack without removing it
STACK_ITEM peek(Stack* stack)
{
    if (Empty(stack))
        return placeholder;
    return stack->array[stack->top];
}