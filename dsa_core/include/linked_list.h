#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"

// Node structure for dynamic storage
typedef struct Node {
    ParkingRecord record;
    struct Node *next;
} Node;

// Linked List controller
typedef struct {
    Node *head;
    int size;
} LinkedList;

// Linked List & Search operations
void init_list(LinkedList *list);
void insert_record(LinkedList *list, ParkingRecord rec);
Node* search_record_by_vehicle(const LinkedList *list, const char *vehicle_number);
void update_violation_score(LinkedList *list, const char *vehicle_number, int score_to_add);
void display_all_records(const LinkedList *list);
void free_list(LinkedList *list);

#endif // LINKED_LIST_H