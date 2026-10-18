#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"

typedef struct Node {
    ActiveSlotRecord record;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int active_count;
} ActiveOccupancyList;

void init_active_list(ActiveOccupancyList *list);
void insert_active_vehicle(ActiveOccupancyList *list, const char *vehicle_num, int slot_id);
int remove_active_vehicle(ActiveOccupancyList *list, const char *vehicle_num);
Node* search_active_vehicle(const ActiveOccupancyList *list, const char *vehicle_num);
void display_active_vehicles(const ActiveOccupancyList *list);
void free_active_list(ActiveOccupancyList *list);

#endif // LINKED_LIST_H