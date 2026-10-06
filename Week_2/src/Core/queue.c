#include <stdio.h>

#include "queue.h"


void queue_init(Queue *queue)
{
    if (queue == NULL)
    {
        return;
    }

    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
}


void queue_reset(Queue *queue)
{
    queue_init(queue);
}


int queue_enqueue(
    Queue *queue,
    int value)
{
    if (queue == NULL)
    {
        return 0;
    }

    if (queue->count >= QUEUE_CAPACITY)
    {
        printf("Queue: queue is full\n");
        return 0;
    }

    queue->data[queue->rear] = value;

    queue->rear =
        (queue->rear + 1) % QUEUE_CAPACITY;

    queue->count++;

    return 1;
}


int queue_dequeue(
    Queue *queue,
    int *value)
{
    if (queue == NULL || value == NULL)
    {
        return 0;
    }

    if (queue->count <= 0)
    {
        printf("Queue: queue is empty\n");
        return 0;
    }

    *value = queue->data[queue->front];

    queue->front =
        (queue->front + 1) % QUEUE_CAPACITY;

    queue->count--;

    return 1;
}


int queue_peek(
    const Queue *queue,
    int *value)
{
    if (queue == NULL || value == NULL)
    {
        return 0;
    }

    if (queue->count <= 0)
    {
        printf("Queue: queue is empty\n");
        return 0;
    }

    *value = queue->data[queue->front];

    return 1;
}