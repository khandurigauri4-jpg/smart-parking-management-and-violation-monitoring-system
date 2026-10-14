#include "linked_list.h"

void init_active_list(ActiveOccupancyList *list) {
    list->head = NULL;
    list->active_count = 0;
}

void insert_active_vehicle(ActiveOccupancyList *list, const char *vehicle_num, int slot_id) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    strncpy(new_node->record.vehicle_number, vehicle_num, VEHICLE_NUM_LEN);
    new_node->record.slot_id = slot_id;
    new_node->next = list->head;
    list->head = new_node;
    list->active_count++;
    printf("[Active Occupancy Index] Parked %s in Slot %d.\n", vehicle_num, slot_id);
}

int remove_active_vehicle(ActiveOccupancyList *list, const char *vehicle_num) {
    Node *curr = list->head;
    Node *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->record.vehicle_number, vehicle_num) == 0) {
            if (prev == NULL) {
                list->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            list->active_count--;
            printf("[Active Occupancy Index] Vehicle %s removed on exit.\n", vehicle_num);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

Node* search_active_vehicle(const ActiveOccupancyList *list, const char *vehicle_num) {
    Node *curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->record.vehicle_number, vehicle_num) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void display_active_vehicles(const ActiveOccupancyList *list) {
    if (list->head == NULL) {
        printf("[Active Occupancy Index] No active vehicles currently parked.\n");
        return;
    }
    printf("\n--- SLIM ACTIVE OCCUPANCY INDEX (%d Parked) ---\n", list->active_count);
    Node *curr = list->head;
    while (curr != NULL) {
        printf("Vehicle Plate: %-12s | Occupied Slot: %d\n", curr->record.vehicle_number, curr->record.slot_id);
        curr = curr->next;
    }
    printf("-----------------------------------------------\n");
}

void free_active_list(ActiveOccupancyList *list) {
    Node *curr = list->head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    list->head = NULL;
    list->active_count = 0;
}