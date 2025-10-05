#include <stdio.h>
#include <stdlib.h>

#define INF 99999 // Representing infinity

// Function to print the path from i to j
void printPath(int **next, int i, int j)
{
    if (i == j)
    {
        printf("%d", i);
        return;
    }
    if (next[i][j] == -1)
    {
        printf("No path");
        return;
    }
    printf("%d", i);
    while (i != j)
    {
        i = next[i][j];
        printf(" -> %d", i);
    }
}

// Function to print all shortest paths and distances
void printAllPaths(int **dist, int **next, int V)
{
    printf("\nAll pairs shortest paths and distances:\n");
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            printf("From %d to %d: ", i, j);
            if (dist[i][j] == INF)
            {
                printf("No path\n");
            }
            else
            {
                printf("Distance = %d, Path = ", dist[i][j]);
                printPath(next, i, j);
                printf("\n");
            }
        }
    }
}

int main()
{
    int V, E;
    printf("Enter number of vertices: ");
    scanf("%d", &V);

    // Allocate distance and next matrices
    int **dist = (int **)malloc(V * sizeof(int *));
    int **next = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
    {
        dist[i] = (int *)malloc(V * sizeof(int));
        next[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                dist[i][j] = 0;
            else
                dist[i][j] = INF;
            next[i][j] = (i == j || dist[i][j] == INF) ? -1 : j;
        }
    }

    printf("Enter number of edges: ");
    scanf("%d", &E);

    printf("Enter edges in format: source destination weight\n");
    for (int i = 0; i < E; i++)
    {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        dist[u][v] = w;
        next[u][v] = v;
    }

    // Floyd-Warshall Algorithm with path reconstruction
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // Check for negative cycles
    int negativeCycle = 0;
    for (int i = 0; i < V; i++)
    {
        if (dist[i][i] < 0)
        {
            negativeCycle = 1;
            break;
        }
    }

    if (negativeCycle)
        printf("\nGraph contains a negative weight cycle!\n");
    else
        printAllPaths(dist, next, V);

    // Free memory
    for (int i = 0; i < V; i++)
    {
        free(dist[i]);
        free(next[i]);
    }
    free(dist);
    free(next);

    return 0;
}
