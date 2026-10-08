#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

#define QUEUE_CAPACITY 50

typedef struct {
    Vehicle items[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} Queue;

// Queue operations
void init_queue(Queue *q);
int is_queue_empty(const Queue *q);
int is_queue_full(const Queue *q);
int enqueue(Queue *q, Vehicle v);
int dequeue(Queue *q, Vehicle *out_v);
int peek_queue(const Queue *q, Vehicle *out_v);
void display_queue(const Queue *q);

#endif // QUEUE_H