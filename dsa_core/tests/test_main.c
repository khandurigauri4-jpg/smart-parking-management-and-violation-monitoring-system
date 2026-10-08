#include <stdio.h>
#include "queue.h"
#include "stack.h"
#include "linked_list.h"

int main() {
    printf("==================================================\n");
    printf("   SMART PARKING & VIOLATION SYSTEM — C DSA TEST  \n");
    printf("==================================================\n\n");

    // 1. TEST WAITING QUEUE MODULE
    printf("--- 1. TESTING WAITING QUEUE ---\n");
    Queue waiting_q;
    init_queue(&waiting_q);

    Vehicle v1 = {"UK07-AB-1234", "10:00 AM"};
    Vehicle v2 = {"DL01-XY-9876", "10:05 AM"};

    enqueue(&waiting_q, v1);
    enqueue(&waiting_q, v2);
    display_queue(&waiting_q);

    Vehicle popped_v;
    if (dequeue(&waiting_q, &popped_v)) {
        printf("[Action] Dequeued vehicle for allocation: %s\n", popped_v.vehicle_number);
    }
    display_queue(&waiting_q);


    // 2. TEST RECENTLY EXITED STACK MODULE
    printf("\n--- 2. TESTING RECENTLY EXITED STACK ---\n");
    Stack exit_stack;
    init_stack(&exit_stack);

    ParkingRecord rec1 = {"UK07-AB-1234", 102, "10:00 AM", "11:30 AM", 45.0, 1, 0, 0};
    ParkingRecord rec2 = {"DL01-XY-9876", 105, "10:05 AM", "12:15 PM", 60.0, 1, 0, 0};

    push_stack(&exit_stack, rec1);
    push_stack(&exit_stack, rec2);
    display_stack(&exit_stack);

    ParkingRecord peeked;
    if (peek_stack(&exit_stack, &peeked)) {
        printf("[Peek] Latest exited vehicle: %s from Slot %d\n", peeked.vehicle_number, peeked.slot_id);
    }


    // 3. TEST LINKED LIST DATABASE & TRAVERSAL SEARCH MODULE
    printf("\n--- 3. TESTING LINKED LIST & TRAVERSAL SEARCH ---\n");
    LinkedList db;
    init_list(&db);

    insert_record(&db, rec1);
    insert_record(&db, rec2);
    display_all_records(&db);

    printf("\n--> Executing Linked List Traversal Search for 'UK07-AB-1234'...\n");
    Node *found = search_record_by_vehicle(&db, "UK07-AB-1234");
    if (found != NULL) {
        printf("[SUCCESS] Record Found!\n");
        printf("          Vehicle: %s | Slot: %d | Fee: $%.2f | Blacklisted: %s\n",
               found->record.vehicle_number,
               found->record.slot_id,
               found->record.fee,
               found->record.is_blacklisted ? "YES" : "NO");
    } else {
        printf("[FAILED] Record not found.\n");
    }


    // 4. TEST VIOLATION MANAGEMENT & AUTOMATIC BLACKLISTING
    printf("\n--- 4. TESTING VIOLATION SCORE & BLACKLISTING ---\n");
    update_violation_score(&db, "UK07-AB-1234", 10); // Score becomes 10
    update_violation_score(&db, "UK07-AB-1234", 6);  // Score becomes 16 (>= 15 -> Blacklisted)

    display_all_records(&db);

    // Clean up dynamic memory
    free_list(&db);

    printf("\n==================================================\n");
    printf("      ALL DSA MODULE TESTS COMPLETED SUCCESSFULLY \n");
    printf("==================================================\n");

    return 0;
}