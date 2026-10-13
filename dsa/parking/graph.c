#include <stdio.h>
#include "graph.h"

static void addEdge(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    int u, int v, int distance)
{
    graph[u][v] = distance;
    graph[v][u] = distance;
}

void initializeGraph(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    int count)
{
    int vertices = count + 1;

    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            graph[i][j] = (i == j) ? 0 : INF;

    /* Entrance (0) to Slot 1 */
    if (count >= 1)
        addEdge(graph, 0, 1, 2);

    /* Horizontal connections in two rows of five slots. */
    for (int i = 1; i < count; i++) {
        if (i != 5)
            addEdge(graph, i, i + 1, 2);
    }

    /* Vertical aisle connections: 1-6, 2-7, ... 5-10. */
    for (int i = 1; i <= 5 && i + 5 <= count; i++)
        addEdge(graph, i, i + 5, 4);
}

int findShortestPath(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    int start,
    int target,
    int count,
    int path[],
    int *pathLength,
    int *totalDistance)
{
    int vertices = count + 1;
    int distance[MAX_GRAPH_VERTICES];
    int previous[MAX_GRAPH_VERTICES];
    int visited[MAX_GRAPH_VERTICES] = {0};

    if (start < 0 || start > count || target < 0 || target > count)
        return 0;

    for (int i = 0; i < vertices; i++) {
        distance[i] = INF;
        previous[i] = -1;
    }
    distance[start] = 0;

    for (int step = 0; step < vertices; step++) {
        int u = -1;
        int minimum = INF;

        for (int i = 0; i < vertices; i++) {
            if (!visited[i] && distance[i] < minimum) {
                minimum = distance[i];
                u = i;
            }
        }
        if (u == -1) break;
        if (u == target) break;

        visited[u] = 1;
        for (int v = 0; v < vertices; v++) {
            if (graph[u][v] != INF && !visited[v] &&
                distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                previous[v] = u;
            }
        }
    }

    if (distance[target] == INF) return 0;

    int reversePath[MAX_GRAPH_VERTICES];
    int length = 0;
    for (int at = target; at != -1; at = previous[at]) {
        reversePath[length++] = at;
        if (at == start) break;
    }
    if (reversePath[length - 1] != start) return 0;

    for (int i = 0; i < length; i++)
        path[i] = reversePath[length - 1 - i];

    *pathLength = length;
    *totalDistance = distance[target];
    return 1;
}

int findNearestAvailableSlot(
    int graph[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
    ParkingSlot slots[],
    int count)
{
    int nearestSlot = -1;
    int shortestDistance = INF;

    for (int i = 0; i < count; i++) {
        if (slots[i].occupied != 0) continue;

        int path[MAX_GRAPH_VERTICES];
        int pathLength = 0;
        int distance = INF;

        if (findShortestPath(graph, 0, slots[i].slotId, count,
                             path, &pathLength, &distance) &&
            distance < shortestDistance) {
            shortestDistance = distance;
            nearestSlot = slots[i].slotId;
        }
    }

    return nearestSlot;
}
