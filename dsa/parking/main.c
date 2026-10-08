#include <stdio.h>
#include "parking.h"


int main()
{
    ParkingSlot slots[MAX_SLOTS];

    int totalSlots = 10;
    int choice;
    int slotId;
    char vehicleNumber[20];

    /* Initialize parking slots */
    initializeSlots(slots, totalSlots);


    while (1)
    {
        printf("\n====================================\n");
        printf("     SMART PARKING MANAGEMENT\n");
        printf("====================================\n");

        printf("1. Park Vehicle\n");
        printf("2. Release Vehicle\n");
        printf("3. Display Parking Status\n");
        printf("4. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);


        /* Park Vehicle */
        if (choice == 1)
        {
            printf("\nEnter vehicle number: ");
            scanf("%19s", vehicleNumber);

            slotId = allocateSlot(
                slots,
                totalSlots,
                vehicleNumber
            );

            if (slotId != -1)
            {
                printf("\nVehicle %s allocated to Slot %d.\n",
                       vehicleNumber,
                       slotId);
            }
            else
            {
                printf("\nSorry! No parking slot is available.\n");
            }
        }


        /* Release Vehicle */
        else if (choice == 2)
        {
            printf("\nEnter slot number to release: ");
            scanf("%d", &slotId);

            releaseSlot(
                slots,
                totalSlots,
                slotId
            );

            printf("\nSlot %d has been released.\n", slotId);
        }


        /* Display Parking */
        else if (choice == 3)
        {
            displaySlots(
                slots,
                totalSlots
            );
        }


        /* Exit */
        else if (choice == 4)
        {
            printf("\nThank you for using Smart Parking System!\n");
            break;
        }


        /* Invalid choice */
        else
        {
            printf("\nInvalid choice. Please try again.\n");
        }
    }


    return 0;
}