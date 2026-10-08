#ifndef STACK_H
#define STACK_H

#include "common.h"

#define STACK_CAPACITY 50

typedef struct {
    ParkingRecord records[STACK_CAPACITY];
    int top;
} Stack;

// Stack operations
void init_stack(Stack *s);
int is_stack_empty(const Stack *s);
int is_stack_full(const Stack *s);
int push_stack(Stack *s, ParkingRecord rec);
int pop_stack(Stack *s, ParkingRecord *out_rec);
int peek_stack(const Stack *s, ParkingRecord *out_rec);
void display_stack(const Stack *s);

#endif // STACK_H