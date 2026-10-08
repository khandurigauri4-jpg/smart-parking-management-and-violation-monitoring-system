#include "linked_list.h"

void init_list(LinkedList *list) {
    list->head = NULL;
    list->size = 0;
}

void insert_record(LinkedList *list, ParkingRecord rec) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("[Error] Memory allocation failed for Linked List node!\n");
        return;
    }
    new_node->record = rec;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
    printf("[LinkedList] Record saved for vehicle %s.\n", rec.vehicle_number);
}

// SEARCH MODULE: Traversal over Linked List nodes
Node* search_record_by_vehicle(const LinkedList *list, const char *vehicle_number) {
    Node *curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->record.vehicle_number, vehicle_number) == 0) {
            return curr; // Vehicle record match found
        }
        curr = curr->next;
    }
    return NULL; // Vehicle record not found
}

void update_violation_score(LinkedList *list, const char *vehicle_number, int score_to_add) {
    Node *target = search_record_by_vehicle(list, vehicle_number);
    if (target != NULL) {
        target->record.violation_score += score_to_add;
        if (target->record.violation_score >= 15) {
            target->record.is_blacklisted = 1;
            printf("[VIOLATION ALERT] Vehicle %s score reached %d! Status: BLACKLISTED.\n",
                   vehicle_number, target->record.violation_score);
        } else {
            printf("[VIOLATION] Vehicle %s updated score: %d\n",
                   vehicle_number, target->record.violation_score);
        }
    } else {
        printf("[Error] Vehicle %s not found in records to update violation.\n", vehicle_number);
    }
}

void display_all_records(const LinkedList *list) {
    if (list->head == NULL) {
        printf("[LinkedList] Database is empty.\n");
        return;
    }
    printf("\n=== ALL PARKING & VIOLATION RECORDS ===\n");
    Node *curr = list->head;
    while (curr != NULL) {
        printf("Vehicle: %-12s | Slot: %-2d | Entry: %-8s | Exit: %-8s | Fee: $%-5.2f | Score: %-2d | Blacklisted: %s\n",
               curr->record.vehicle_number,
               curr->record.slot_id,
               curr->record.entry_time,
               curr->record.exit_time,
               curr->record.fee,
               curr->record.violation_score,
               curr->record.is_blacklisted ? "YES" : "NO");
        curr = curr->next;
    }
    printf("=========================================\n");
}

void free_list(LinkedList *list) {
    Node *curr = list->head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
}