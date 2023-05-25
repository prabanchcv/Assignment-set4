#include <stdio.h>
#include <stdlib.h>

struct Graph {
    int** adjacencyList;
    int* discoveryTime;
    int* lowTime;
    int* visited;
    int* parent;
    int** criticalConnections;
    int criticalConnectionsCount;
    int time;
};

struct Graph* createGraph(int n, int connectionsSize, int** connections) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    graph->adjacencyList = (int**)malloc(n * sizeof(int*));
    graph->discoveryTime = (int*)malloc(n * sizeof(int));
    graph->lowTime = (int*)malloc(n * sizeof(int));
    graph->visited = (int*)malloc(n * sizeof(int));
    graph->parent = (int*)malloc(n * sizeof(int));
    graph->criticalConnections = NULL;
    graph->criticalConnectionsCount = 0;
    graph->time = 0;

    for (int i = 0; i < n; i++) {
        graph->adjacencyList[i] = NULL;
        graph->discoveryTime[i] = -1;
        graph->lowTime[i] = -1;
        graph->visited[i] = 0;
        graph->parent[i] = -1;
    }

    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];

        if (graph->adjacencyList[u] == NULL) {
            graph->adjacencyList[u] = (int*)malloc(n * sizeof(int));
            for (int j = 0; j < n; j++) {
                graph->adjacencyList[u][j] = -1;
            }
        }
        if (graph->adjacencyList[v] == NULL) {
            graph->adjacencyList[v] = (int*)malloc(n * sizeof(int));
            for (int j = 0; j < n; j++) {
                graph->adjacencyList[v][j] = -1;
            }
        }

        int j = 0;
        while (graph->adjacencyList[u][j] != -1) {
            j++;
        }
        graph->adjacencyList[u][j] = v;

        j = 0;
        while (graph->adjacencyList[v][j] != -1) {
            j++;
        }
        graph->adjacencyList[v][j] = u;
    }

    return graph;
}

void dfs(struct Graph* graph, int u) {
    graph->visited[u] = 1;
    graph->discoveryTime[u] = graph->lowTime[u] = ++graph->time;

    int vIndex = 0;
    while (graph->adjacencyList[u][vIndex] != -1) {
        int v = graph->adjacencyList[u][vIndex];

        if (graph->visited[v] == 0) {
            graph->parent[v] = u;
            dfs(graph, v);

            graph->lowTime[u] = (graph->lowTime[u] < graph->lowTime[v]) ? graph->lowTime[u] : graph->lowTime[v];

            if (graph->discoveryTime[u] < graph->lowTime[v]) {
                graph->criticalConnectionsCount++;

                graph->criticalConnections = (int**)realloc(graph->criticalConnections, graph->criticalConnectionsCount * sizeof(int*));
                graph->criticalConnections[graph->criticalConnectionsCount - 1] = (int*)malloc(2 * sizeof(int));
                graph->criticalConnections[graph->criticalConnectionsCount - 1][0] = u;
                graph->criticalConnections[graph->criticalConnectionsCount - 1][1] = v;
            }
        } else if (v != graph->parent[u]) {
            graph->lowTime[u] = (graph->lowTime[u] < graph->discoveryTime[v]) ? graph->lowTime[u] : graph->discoveryTime[v];
        }

        vIndex++;
    }
}

int** criticalConnections(int n, int connectionsSize, int** connections, int* returnSize, int** returnColumnSizes) {
    struct Graph* graph = createGraph(n, connectionsSize, connections);

    for (int i = 0; i < n; i++) {
        if (graph->visited[i] == 0) {
            dfs(graph, i);
        }
    }

    *returnSize = graph->criticalConnectionsCount;
    *returnColumnSizes = (int*)malloc(graph->criticalConnectionsCount * sizeof(int));
    int** result = (int**)malloc(graph->criticalConnectionsCount * sizeof(int*));

    for (int i = 0; i < graph->criticalConnectionsCount; i++) {
        (*returnColumnSizes)[i] = 2;
        result[i] = (int*)malloc(2 * sizeof(int));
        result[i][0] = graph->criticalConnections[i][0];
        result[i][1] = graph->criticalConnections[i][1];
    }

    return result;
}

int main() {
    int n = 4;
    int connectionsSize = 4;
    int** connections = (int**)malloc(connectionsSize * sizeof(int*));
    for (int i = 0; i < connectionsSize; i++) {
        connections[i] = (int*)malloc(2 * sizeof(int));
    }

    connections[0][0] = 0;
    connections[0][1] = 1;
    connections[1][0] = 1;
    connections[1][1] = 2;
    connections[2][0] = 2;
    connections[2][1] = 0;
    connections[3][0] = 1;
    connections[3][1] = 3;

    int returnSize;
    int* returnColumnSizes;
    int** result = criticalConnections(n, connectionsSize, connections, &returnSize, &returnColumnSizes);

    for (int i = 0; i < returnSize; i++) {
        printf("[%d, %d]\n", result[i][0], result[i][1]);
    }

    return 0;
}

