#ifndef PARKING_H
#define PARKING_H

#define MAX_SLOTS 20

typedef struct
{
    int slotId;
    int occupied;
    char vehicleNumber[20];
} ParkingSlot;


/* Initialize all parking slots */
void initializeSlots(ParkingSlot slots[], int count);


/* Display parking status */
void displaySlots(ParkingSlot slots[], int count);


/* Allocate first available slot */
int allocateSlot(ParkingSlot slots[], int count,
                 const char vehicleNumber[]);


/* Release a parking slot */
void releaseSlot(ParkingSlot slots[], int count, int slotId);

#endif