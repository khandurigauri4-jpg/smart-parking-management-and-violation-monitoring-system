#include "stack.h"

void init_stack(Stack *s) {
    s->top = -1;
}

int is_stack_empty(const Stack *s) {
    return s->top == -1;
}

int is_stack_full(const Stack *s) {
    return s->top == STACK_CAPACITY - 1;
}

int push_stack(Stack *s, ParkingRecord rec) {
    if (is_stack_full(s)) {
        printf("[Error] Exited Vehicles Stack is full!\n");
        return 0;
    }
    s->records[++(s->top)] = rec;
    printf("[Stack] Vehicle %s pushed to recent exit history.\n", rec.vehicle_number);
    return 1;
}

int pop_stack(Stack *s, ParkingRecord *out_rec) {
    if (is_stack_empty(s)) {
        printf("[Stack] History stack is empty.\n");
        return 0;
    }
    if (out_rec != NULL) {
        *out_rec = s->records[(s->top)--];
    } else {
        s->top--;
    }
    return 1;
}

int peek_stack(const Stack *s, ParkingRecord *out_rec) {
    if (is_stack_empty(s)) {
        return 0;
    }
    if (out_rec != NULL) {
        *out_rec = s->records[s->top];
    }
    return 1;
}

void display_stack(const Stack *s) {
    if (is_stack_empty(s)) {
        printf("[Stack] No recently exited vehicles.\n");
        return;
    }
    printf("\n--- Recently Exited Vehicles (LIFO) ---\n");
    for (int i = s->top; i >= 0; i--) {
        printf("Vehicle: %s | Slot: %d | Exit Time: %s | Fee: $%.2f\n",
               s->records[i].vehicle_number,
               s->records[i].slot_id,
               s->records[i].exit_time,
               s->records[i].fee);
    }
    printf("---------------------------------------\n");
}