#include <stdio.h>
#include "queue.h"
#include "stack.h"
#include "linked_list.h"

int main() {
    printf("========================================================\n");
    printf("   SMART PARKING SYSTEM — C DSA ENGINE (3 MODULES)     \n");
    printf("========================================================\n\n");

    // 1. PRE-BOOKING QUEUE MODULE
    printf("--- 1. TESTING PRE-BOOKING RESERVATION QUEUE ---\n");
    ReservationQueue res_q;
    init_queue(&res_q);

    Reservation r1 = {"UK07-AB-1234", "10:30 AM", 1};
    Reservation r2 = {"DL01-XY-9876", "11:00 AM", 2};
    enqueue_reservation(&res_q, r1);
    enqueue_reservation(&res_q, r2);
    display_reservations(&res_q);

    Reservation dequeued_r;
    dequeue_reservation(&res_q, &dequeued_r);
    printf("Allocating slot for arrived pre-booking: %s\n", dequeued_r.vehicle_number);


    // 2. SLIM LINKED LIST ACTIVE OCCUPANCY INDEX
    printf("\n--- 2. TESTING ACTIVE OCCUPANCY INDEX (LINKED LIST) ---\n");
    ActiveOccupancyList active_db;
    init_active_list(&active_db);

    insert_active_vehicle(&active_db, "UK07-AB-1234", 5);
    insert_active_vehicle(&active_db, "DL01-XY-9876", 8);
    display_active_vehicles(&active_db);

    Node *search_res = search_active_vehicle(&active_db, "UK07-AB-1234");
    if (search_res) {
        printf("[Search Match] Vehicle %s currently in Slot %d\n",
               search_res->record.vehicle_number, search_res->record.slot_id);
    }


    // 3. GATE OPERATOR UNDO STACK
    printf("\n--- 3. TESTING GATE AUDIT & UNDO STACK ---\n");
    UndoStack audit_stack;
    init_stack(&audit_stack);

    GateAction a1 = {"UK07-AB-1234", 5, "ENTRY"};
    GateAction a2 = {"DL01-XY-9876", 8, "OVERRIDE"};

    push_action(&audit_stack, a1);
    push_action(&audit_stack, a2);
    display_undo_stack(&audit_stack);

    GateAction popped_action;
    pop_action(&audit_stack, &popped_action);
    printf("[Operator Action] Reverted action: %s for %s\n", popped_action.action_type, popped_action.vehicle_number);

    free_active_list(&active_db);

    printf("\n========================================================\n");
    printf("   DSA ENGINE COMPLETED ALL 3 MODULE TESTS CLEANLY      \n");
    printf("========================================================\n");

    return 0;
}