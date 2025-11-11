#include <stdio.h>
#include <stdbool.h>

#define MAX 100 // maximum number of vertices on each side

int nU, nV;          // Number of nodes in set U and V
int graph[MAX][MAX]; // Adjacency matrix: graph[u][v] = 1 if edge exists
int matchV[MAX];     // matchV[v] = u matched with v, or -1 if free
bool used[MAX];      // visited array for DFS

// DFS to find augmenting path
bool dfs(int u)
{
    for (int v = 0; v < nV; v++)
    {
        if (graph[u][v] && !used[v])
        {
            used[v] = true;

            // If v is free or we can re-match its current partner
            if (matchV[v] == -1 || dfs(matchV[v]))
            {
                matchV[v] = u;
                return true;
            }
        }
    }
    return false;
}

int maxMatching()
{
    int result = 0;

    // Initialize all vertices of V as free
    for (int i = 0; i < nV; i++)
        matchV[i] = -1;

    // Try to find matching for every u in U
    for (int u = 0; u < nU; u++)
    {
        for (int i = 0; i < nV; i++)
            used[i] = false; // Reset visited before each DFS

        if (dfs(u))
            result++;
    }
    return result;
}

int main()
{
    printf("Enter number of nodes in U and V: ");
    scanf("%d %d", &nU, &nV);

    printf("Enter adjacency matrix (U x V):\n");
    for (int i = 0; i < nU; i++)
        for (int j = 0; j < nV; j++)
            scanf("%d", &graph[i][j]);

    int matching = maxMatching();
    printf("\nMaximum Bipartite Matching = %d\n", matching);

    printf("Matched pairs (U -> V):\n");
    for (int v = 0; v < nV; v++)
    {
        if (matchV[v] != -1)
            printf("U%d -> V%d\n", matchV[v], v);
    }

    return 0;
}
