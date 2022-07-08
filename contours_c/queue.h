#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;
  
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue* createQueue(unsigned capacity);
  
// Queue is full when size becomes
// equal to the capacity
int isFull(Queue* queue);
  
// Queue is empty when size is 0
int isEmpty(Queue* queue);
  
// Function to add an item to the queue.
// It changes rear and size
void enqueue(Queue* queue, int item);
  
// Function to remove an item from queue.
// It changes front and size
int dequeue(Queue* queue);
  
// Function to get front of queue
int front(Queue* queue);
  
// Function to get rear of queue
int rear(Queue* queue);
  
#endif