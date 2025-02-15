#include "queue.h"

const Point p = {-1, -1};

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue* createQueue(unsigned capacity)
{
    Queue* queue = (Queue*)malloc(
        sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
  
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (ITEM*)malloc(
        queue->capacity * sizeof(ITEM));
    return queue;
}
  
// Queue is full when size becomes
// equal to the capacity
int isFull(Queue* queue)
{
    return (queue->size == queue->capacity);
}
  
// Queue is empty when size is 0
int isEmpty(Queue* queue)
{
    return (queue->size == 0);
}
  
// Function to add an item to the queue.
// It changes rear and size
void enqueue(Queue* queue, ITEM item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    // printf("%d enqueued to queue\n", item);
}
  
// Function to remove an item from queue.
// It changes front and size
ITEM dequeue(Queue* queue)
{
    if (isEmpty(queue))
        return p;
    ITEM item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
  
// Function to get front of queue
ITEM front(Queue* queue)
{
    if (isEmpty(queue))
        return p;
    return queue->array[queue->front];
}
  
// Function to get rear of queue
ITEM rear(Queue* queue)
{
    if (isEmpty(queue))
        return p;
    return queue->array[queue->rear];
}