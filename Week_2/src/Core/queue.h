#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_CAPACITY 100

typedef struct
{
    int data[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} Queue;

void queue_init(Queue *queue);
void queue_reset(Queue *queue);

int queue_enqueue(
    Queue *queue,
    int value
);

int queue_dequeue(
    Queue *queue,
    int *value
);

int queue_peek(
    const Queue *queue,
    int *value
);

#endif