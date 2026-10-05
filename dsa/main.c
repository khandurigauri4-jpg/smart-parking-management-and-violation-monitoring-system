#include <stdio.h>
#include "parking/parking.h"
#include "searching/searching.h"
#include "sorting/sorting.h"

int main()
{
    ParkingSlot slots[MAX_SLOTS];
    int totalSlots = 10;

    initializeSlots(slots, totalSlots);

    printf("SMART PARKING DSA ENGINE\n");

    int slot1 = allocateSlot(slots, totalSlots, "UK07AB1234");
    int slot2 = allocateSlot(slots, totalSlots, "UK07CD5678");

    printf("\nVehicle UK07AB1234 allocated to Slot %d\n", slot1);
    printf("Vehicle UK07CD5678 allocated to Slot %d\n", slot2);

    displaySlots(slots, totalSlots);

    int foundSlot = searchVehicle(slots, totalSlots, "UK07CD5678");

    if (foundSlot != -1)
    {
        printf("\nVehicle UK07CD5678 found at Slot %d\n", foundSlot);
    }
    else
    {
        printf("\nVehicle not found\n");
    }

    releaseSlot(slots, totalSlots, slot1);

    printf("\nAfter vehicle exit:\n");
    displaySlots(slots, totalSlots);
    sortAvailableSlots(slots, totalSlots);

printf("\nSlots after sorting:\n");
displaySlots(slots, totalSlots);

    return 0;
}