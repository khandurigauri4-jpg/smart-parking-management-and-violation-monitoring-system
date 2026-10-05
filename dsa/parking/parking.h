#ifndef PARKING_H
#define PARKING_H

#define MAX_SLOTS 20

typedef struct {
    int slotId;
    int occupied;
    char vehicleNumber[20];
} ParkingSlot;

void initializeSlots(ParkingSlot slots[], int count);
void displaySlots(ParkingSlot slots[], int count);
int allocateSlot(ParkingSlot slots[], int count, const char vehicleNumber[]);
void releaseSlot(ParkingSlot slots[], int count, int slotId);

#endif