#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEHICLE_NUM_LEN 16
#define TIMESTAMP_LEN 32

// 1. Queue Data Model: Online Pre-Booking Reservation
typedef struct {
    char vehicle_number[VEHICLE_NUM_LEN];
    char reservation_time[TIMESTAMP_LEN];
    int slot_type; // 1: Standard, 2: EV/VIP
} Reservation;

// 2. Stack Data Model: Gate Operator Action / Undo Event
typedef struct {
    char vehicle_number[VEHICLE_NUM_LEN];
    int slot_id;
    char action_type[16]; // "ENTRY", "EXIT", "OVERRIDE"
} GateAction;

// 3. Linked List Data Model: Slim Active Occupancy Index
typedef struct {
    char vehicle_number[VEHICLE_NUM_LEN];
    int slot_id;
} ActiveSlotRecord;

#endif // COMMON_H