#include "ShortestPath.h"

int Graph_getNodeIndex(Graph* graph, const char* nodeId) {
    printf("nodeId : %s\n", nodeId);

    NodesList* current = graph->nodes;
    int index = 0;
    while (current != NULL && current->node != NULL) {
        if (strcmp(current->node->id, nodeId) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1; // Node not found
}

char* Graph_getNodeId(Graph* graph, int index) {
    printf("node index : %d\n", index);

    NodesList* current = graph->nodes;
    int i = 0;
    while (current != NULL && current->node != NULL) {
        if (i == index) {
            return _strdup(current->node->id);
        }
        current = current->next;
        i++;
    }
    return NULL; // Node not found
}

Path* Graph_shortestPath(Graph* graph, const char* startId, const char* endId) {
    int start = Graph_getNodeIndex(graph, startId);
    int end = Graph_getNodeIndex(graph, endId);

    assert(!(start == -1 || end == -1));

    int size = graph->nodesCount;
    int* predecessors = (int*)calloc(size, sizeof(int));
    AssertNew(predecessors);

    float* distances = (float*)calloc(size, sizeof(float));
    AssertNew(distances);

    Graph_dijkstra(graph, start, end, predecessors, distances);

    Path* path = Graph_dijkstraGetPath((Graph*)graph, predecessors, distances, end);

    free(predecessors);
    predecessors = NULL;
    free(distances);
    distances = NULL;

    return path;
}

void Graph_dijkstra(Graph* graph, int start, int end, int* predecessors, float* distances) {
    int size = graph->nodesCount;

    bool* explored = (bool*)calloc(size, sizeof(bool));
    if (explored == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        predecessors[i] = -1;
        distances[i] = INFINITY;
    }
    distances[start] = 0.0f;

    while (true) {
        int currID = -1;
        float currDist = INFINITY;
        for (int i = 0; i < size; ++i) {
            if (!explored[i] && distances[i] < currDist) {
                currDist = distances[i];
                currID = i;
            }
        }

        if (currID < 0 || currID == end) {
            printf("Stopping at node %d\n", currID);
            break;
        }

        explored[currID] = true;

        NodesList* arc = Graph_getArcList(graph, currID);
        while (arc != NULL) {
            int nextID = Graph_getNodeIndex(graph, arc->node->id);
            if (nextID == -1) continue; // Invalid node, skip

            float dist = distances[currID] + arc->weight;
            if (distances[nextID] > dist) {
                printf("Updating distance for node %d: %f -> %f\n", nextID, distances[nextID], dist);
                distances[nextID] = dist;
                predecessors[nextID] = currID;
            }
            arc = arc->next;
        }
    }
    free(explored);
}

Path* Graph_dijkstraGetPath(Graph* graph, int* predecessors, float* distances, int end) {
    assert(graph && predecessors && distances);
    assert(end >= 0);

    if (predecessors[end] < 0) {
        printf("No path to node %d\n", end);
        return NULL;
    }

    char* endId = Graph_getNodeId(graph, end);
    Path* path = Path_create(endId);
    free(endId);

    int currID = end;

    while (predecessors[currID] >= 0) {
        currID = predecessors[currID];
        char* nodeId = Graph_getNodeId(graph, currID);
        ListStr_insertFirst(path->list, nodeId);
        free(nodeId);
    }

    path->distance = distances[end];

    return path;
}

Path* Path_create(const char* startId) {
    Path* path = (Path*)calloc(1, sizeof(Path));
    AssertNew(path);

    path->distance = 0.0f;
    path->list = ListStr_create();
    char* nodeId = _strdup(startId);
    AssertNew(nodeId);
    ListStr_insertLast(path->list, nodeId);

    return path;
}

void Path_destroy(Path* path) {
    if (path == NULL) return;

    ListStr_destroy(path->list);
    free(path);
}

void Path_print(Path* path) {
    if (path == NULL) {
        printf("path: NULL\n");
        return;
    }

    printf("path (distance = %f): ", path->distance);
    ListStr_print(path->list);
}
