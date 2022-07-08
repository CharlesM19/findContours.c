#ifndef _STACK_H
#define _STACK_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"

#define STACK_ITEM Point

extern const Point placeholder;

// A structure to represent a stack
typedef struct Stack {
    int top;
    unsigned capacity;
    STACK_ITEM* array;
} Stack;


// function to create a stack of given capacity. It initializes size of
// stack as 0
Stack* createStack(unsigned capacity);
  
// Stack is full when top is equal to the last index
int Full(Stack* stack);
  
// Stack is empty when top is equal to -1
int Empty(Stack* stack);
  
// Function to add an item to stack.  It increases top by 1
void push(Stack* stack, STACK_ITEM item);
  
// Function to remove an item from stack.  It decreases top by 1
STACK_ITEM pop(Stack* stack);
  
// Function to return the top from stack without removing it
STACK_ITEM peek(Stack* stack);

#endif