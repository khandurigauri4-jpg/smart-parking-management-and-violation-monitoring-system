#include <string.h>
#include "searching.h"

int searchVehicle(ParkingSlot slots[], int count, const char vehicleNumber[])
{
    for (int i = 0; i < count; i++)
    {
        if (slots[i].occupied == 1 &&
            strcmp(slots[i].vehicleNumber, vehicleNumber) == 0)
        {
            return slots[i].slotId;
        }
    }

    return -1;
}