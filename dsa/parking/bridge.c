#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parking.h"
#include "graph.h"

/*
 * Node.js calls this compiled C bridge.
 * Usage:
 *   parking_bridge nearest occupied0 occupied1 ... occupiedN
 *   parking_bridge path targetSlot
 *
 * nearest arguments are 0/1 occupancy flags for slots 1..N.
 * Output is JSON so the Node backend can consume it.
 */
int main(int argc, char *argv[])
{
    const int count = 10;
    ParkingSlot slots[MAX_SLOTS];
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES];

    initializeSlots(slots, count);
    initializeGraph(graph, count);

    if (argc < 2) {
        fprintf(stderr, "Missing bridge command.\n");
        return 2;
    }

    if (strcmp(argv[1], "nearest") == 0) {
        if (argc != count + 2) {
            fprintf(stderr, "Expected 10 occupancy flags.\n");
            return 2;
        }

        for (int i = 0; i < count; i++) {
            int occupied = atoi(argv[i + 2]);
            if (occupied != 0 && occupied != 1) {
                fprintf(stderr, "Invalid occupancy flag.\n");
                return 2;
            }
            slots[i].occupied = occupied;
        }

        int slotId = findNearestAvailableSlot(graph, slots, count);
        if (slotId == -1) {
            printf("{\"slotId\":null,\"message\":\"No available slot.\"}\n");
            return 0;
        }

        int path[MAX_GRAPH_VERTICES];
        int pathLength = 0;
        int distance = 0;
        findShortestPath(graph, 0, slotId, count,
                         path, &pathLength, &distance);

        printf("{\"slotId\":%d,\"distance\":%d,\"path\":[", slotId, distance);
        for (int i = 0; i < pathLength; i++)
            printf("%s%d", i ? "," : "", path[i]);
        printf("]}\n");
        return 0;
    }

    if (strcmp(argv[1], "path") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Expected a target slot.\n");
            return 2;
        }

        int target = atoi(argv[2]);
        if (target < 1 || target > count) {
            fprintf(stderr, "Target slot must be between 1 and 10.\n");
            return 2;
        }

        int path[MAX_GRAPH_VERTICES];
        int pathLength = 0;
        int distance = 0;
        if (!findShortestPath(graph, 0, target, count,
                              path, &pathLength, &distance)) {
            printf("{\"message\":\"No route found.\"}\n");
            return 1;
        }

        printf("{\"slotId\":%d,\"distance\":%d,\"path\":[", target, distance);
        for (int i = 0; i < pathLength; i++)
            printf("%s%d", i ? "," : "", path[i]);
        printf("]}\n");
        return 0;
    }

    fprintf(stderr, "Unknown bridge command.\n");
    return 2;
}
