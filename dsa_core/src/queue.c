#include "queue.h"

void init_queue(ReservationQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int is_queue_empty(const ReservationQueue *q) {
    return q->count == 0;
}

int is_queue_full(const ReservationQueue *q) {
    return q->count == QUEUE_CAPACITY;
}

int enqueue_reservation(ReservationQueue *q, Reservation r) {
    if (is_queue_full(q)) {
        printf("[Error] Pre-booking reservation queue full!\n");
        return 0;
    }
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    q->items[q->rear] = r;
    q->count++;
    printf("[Pre-Booking Queue] Reservation confirmed for %s at %s.\n", r.vehicle_number, r.reservation_time);
    return 1;
}

int dequeue_reservation(ReservationQueue *q, Reservation *out_r) {
    if (is_queue_empty(q)) {
        printf("[Pre-Booking Queue] No pending pre-bookings.\n");
        return 0;
    }
    if (out_r != NULL) {
        *out_r = q->items[q->front];
    }
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->count--;
    return 1;
}

void display_reservations(const ReservationQueue *q) {
    if (is_queue_empty(q)) {
        printf("[Pre-Booking Queue] No active reservations.\n");
        return;
    }
    printf("\n--- PRE-BOOKING RESERVATION QUEUE (%d) ---\n", q->count);
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % QUEUE_CAPACITY;
        printf("%d. Vehicle: %s | Time: %s | Type: %s\n",
               i + 1, q->items[idx].vehicle_number, q->items[idx].reservation_time,
               q->items[idx].slot_type == 2 ? "EV/VIP" : "Standard");
    }
    printf("-----------------------------------------\n");
}