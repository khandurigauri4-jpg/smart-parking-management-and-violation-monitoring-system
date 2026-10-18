#ifndef STACK_H
#define STACK_H

#include "common.h"

#define STACK_CAPACITY 50

typedef struct {
    GateAction actions[STACK_CAPACITY];
    int top;
} UndoStack;

void init_stack(UndoStack *s);
int is_stack_empty(const UndoStack *s);
int push_action(UndoStack *s, GateAction action);
int pop_action(UndoStack *s, GateAction *out_action);
void display_undo_stack(const UndoStack *s);

#endif // STACK_H