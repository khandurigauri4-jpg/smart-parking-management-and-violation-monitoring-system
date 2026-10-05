#include "sorting.h"

void sortAvailableSlots(ParkingSlot slots[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (slots[j].slotId > slots[j + 1].slotId)
            {
                ParkingSlot temp = slots[j];
                slots[j] = slots[j + 1];
                slots[j + 1] = temp;
            }
        }
    }
}