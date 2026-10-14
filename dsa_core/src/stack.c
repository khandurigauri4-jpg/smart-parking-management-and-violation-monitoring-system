#include "stack.h"

void init_stack(UndoStack *s) {
    s->top = -1;
}

int is_stack_empty(const UndoStack *s) {
    return s->top == -1;
}

int push_action(UndoStack *s, GateAction action) {
    if (s->top == STACK_CAPACITY - 1) return 0;
    s->actions[++(s->top)] = action;
    printf("[Audit Stack] Logged action: %s for %s at Slot %d.\n",
           action.action_type, action.vehicle_number, action.slot_id);
    return 1;
}

int pop_action(UndoStack *s, GateAction *out_action) {
    if (is_stack_empty(s)) {
        printf("[Audit Stack] No recent actions to undo.\n");
        return 0;
    }
    if (out_action != NULL) {
        *out_action = s->actions[(s->top)--];
    } else {
        s->top--;
    }
    return 1;
}

void display_undo_stack(const UndoStack *s) {
    if (is_stack_empty(s)) {
        printf("[Audit Stack] Action log empty.\n");
        return;
    }
    printf("\n--- GATE OPERATOR UNDO LOG (LIFO) ---\n");
    for (int i = s->top; i >= 0; i--) {
        printf("[%d] Action: %s | Vehicle: %s | Slot: %d\n",
               i + 1, s->actions[i].action_type, s->actions[i].vehicle_number, s->actions[i].slot_id);
    }
    printf("------------------------------------\n");
}