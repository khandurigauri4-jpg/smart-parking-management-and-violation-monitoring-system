#ifndef GRAPH_H
#define GRAPH_H

#include "parking.h"

#define MAX_GRAPH_VERTICES (MAX_SLOTS + 1)
#define INF 999999

void initializeGraph(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    int count
);

int findNearestAvailableSlot(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    ParkingSlot slots[],
    int count
);

/* Vertices: 0 = entrance; 1..count = parking slots. */
int findShortestPath(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    int start,
    int target,
    int count,
    int path[],
    int *pathLength,
    int *totalDistance
);

#endif
