#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEHICLE_NUM_LEN 16
#define TIMESTAMP_LEN 32

typedef struct {
    char vehicle_number[VEHICLE_NUM_LEN];
    char arrival_time[TIMESTAMP_LEN];
} Vehicle;

typedef struct {
    char vehicle_number[VEHICLE_NUM_LEN];
    int slot_id;
    char entry_time[TIMESTAMP_LEN];
    char exit_time[TIMESTAMP_LEN];
    double fee;
    int is_paid;
    int violation_score;
    int is_blacklisted;
} ParkingRecord;

#endif // COMMON_H