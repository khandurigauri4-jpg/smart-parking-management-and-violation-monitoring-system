#include "queue.h"

void init_queue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int is_queue_empty(const Queue *q) {
    return q->count == 0;
}

int is_queue_full(const Queue *q) {
    return q->count == QUEUE_CAPACITY;
}

int enqueue(Queue *q, Vehicle v) {
    if (is_queue_full(q)) {
        printf("[Error] Waiting queue full! Cannot add vehicle: %s\n", v.vehicle_number);
        return 0;
    }
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    q->items[q->rear] = v;
    q->count++;
    printf("[Queue] Vehicle %s added to waiting queue.\n", v.vehicle_number);
    return 1;
}

int dequeue(Queue *q, Vehicle *out_v) {
    if (is_queue_empty(q)) {
        printf("[Queue] Waiting queue is empty.\n");
        return 0;
    }
    if (out_v != NULL) {
        *out_v = q->items[q->front];
    }
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->count--;
    return 1;
}

int peek_queue(const Queue *q, Vehicle *out_v) {
    if (is_queue_empty(q)) {
        return 0;
    }
    if (out_v != NULL) {
        *out_v = q->items[q->front];
    }
    return 1;
}

void display_queue(const Queue *q) {
    if (is_queue_empty(q)) {
        printf("[Queue] No waiting vehicles.\n");
        return;
    }
    printf("\n--- Waiting Queue (%d vehicles) ---\n", q->count);
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % QUEUE_CAPACITY;
        printf("%d. Vehicle: %s | Arrival: %s\n", 
               i + 1, q->items[idx].vehicle_number, q->items[idx].arrival_time);
    }
    printf("----------------------------------\n");
}