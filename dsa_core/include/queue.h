#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

#define QUEUE_CAPACITY 50

typedef struct {
    Reservation items[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} ReservationQueue;

void init_queue(ReservationQueue *q);
int is_queue_empty(const ReservationQueue *q);
int is_queue_full(const ReservationQueue *q);
int enqueue_reservation(ReservationQueue *q, Reservation r);
int dequeue_reservation(ReservationQueue *q, Reservation *out_r);
void display_reservations(const ReservationQueue *q);

#endif // QUEUE_H