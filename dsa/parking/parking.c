#include <stdio.h>
#include <string.h>
#include "parking.h"


/* ================= INITIALIZE SLOTS ================= */

void initializeSlots(ParkingSlot slots[], int count)
{
    for (int i = 0; i < count; i++)
    {
        slots[i].slotId = i + 1;
        slots[i].occupied = 0;
        strcpy(slots[i].vehicleNumber, "");
    }
}


/* ================= DISPLAY SLOTS ================= */

void displaySlots(ParkingSlot slots[], int count)
{
    printf("\n========== PARKING STATUS ==========\n");

    printf("Slot\tStatus\t\tVehicle\n");
    printf("------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        if (slots[i].occupied == 1)
        {
            printf("%d\tOccupied\t%s\n",
                   slots[i].slotId,
                   slots[i].vehicleNumber);
        }
        else
        {
            printf("%d\tAvailable\t-\n",
                   slots[i].slotId);
        }
    }
}


/* ================= ALLOCATE SLOT ================= */

int allocateSlot(ParkingSlot slots[], int count,
                 const char vehicleNumber[])
{
    for (int i = 0; i < count; i++)
    {
        if (slots[i].occupied == 0)
        {
            slots[i].occupied = 1;

            strcpy(slots[i].vehicleNumber,
                   vehicleNumber);

            return slots[i].slotId;
        }
    }

    return -1;
}


/* ================= RELEASE SLOT ================= */

void releaseSlot(ParkingSlot slots[], int count, int slotId)
{
    for (int i = 0; i < count; i++)
    {
        if (slots[i].slotId == slotId)
        {
            slots[i].occupied = 0;

            strcpy(slots[i].vehicleNumber, "");

            return;
        }
    }
}