
#include <stdio.h>
#include "parking.h"
#include "graph.h"

int main()
{
    ParkingSlot slots[MAX_SLOTS];
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES];

    int totalSlots = 10;
    int choice;
    int slotId;
    char vehicleNumber[20];

    initializeSlots(slots, totalSlots);
    initializeGraph(graph, totalSlots);

    while (1)
    {
        printf("\n====================================\n");
        printf("     SMART PARKING MANAGEMENT\n");
        printf("====================================\n");
        printf("1. Park Vehicle\n");
        printf("2. Release Vehicle\n");
        printf("3. Display Parking Status\n");
        printf("4. Find Nearest Available Slot\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");
            return 1;
        }

        if (choice == 1)
        {
            printf("Enter vehicle number: ");
            scanf("%19s", vehicleNumber);

            slotId = allocateSlot(
                slots, totalSlots, vehicleNumber);

            if (slotId != -1)
                printf("Vehicle %s allocated to Slot %d.\n",
                       vehicleNumber, slotId);
            else
                printf("No parking slot available.\n");
        }
        else if (choice == 2)
        {
            printf("Enter slot number to release: ");

            if (scanf("%d", &slotId) != 1)
            {
                printf("Invalid input.\n");
                return 1;
            }

            if (slotId < 1 || slotId > totalSlots)
            {
                printf("Invalid slot number.\n");
            }
            else if (slots[slotId - 1].occupied == 0)
            {
                printf("Slot is already available.\n");
            }
            else
            {
                releaseSlot(slots, totalSlots, slotId);
                printf("Slot %d released successfully.\n",
                       slotId);
            }
        }
        else if (choice == 3)
        {
            displaySlots(slots, totalSlots);
        }
        else if (choice == 4)
        {
            slotId = findNearestAvailableSlot(
                graph, slots, totalSlots);

            if (slotId == -1)
                printf("No available slot found.\n");
            else
                printf("Nearest available slot: %d\n",
                       slotId);
        }
        else if (choice == 5)
        {
            printf("Thank you for using Smart Parking!\n");
            break;
        }
        else
        {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
