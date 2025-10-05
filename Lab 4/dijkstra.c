#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure for adjacency list node
typedef struct Node
{
    int vertex;
    int weight;
    struct Node *next;
} Node;

// Structure for the graph
typedef struct Graph
{
    int V;
    Node **array;
} Graph;

// Create a new adjacency list node
Node *newNode(int vertex, int weight)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->vertex = vertex;
    node->weight = weight;
    node->next = NULL;
    return node;
}

// Create a graph with V vertices
Graph *createGraph(int V)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (Node **)malloc(V * sizeof(Node *));
    for (int i = 0; i < V; i++)
        graph->array[i] = NULL;
    return graph;
}

// Add edge to a directed graph
void addEdge(Graph *graph, int src, int dest, int weight)
{
    Node *node = newNode(dest, weight);
    node->next = graph->array[src];
    graph->array[src] = node;

    // Uncomment for undirected graph:
    // node = newNode(src, weight);
    // node->next = graph->array[dest];
    // graph->array[dest] = node;
}

// Find vertex with minimum distance
int minDistance(int dist[], int visited[], int V)
{
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < V; v++)
    {
        if (!visited[v] && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Dijkstra's algorithm
void dijkstra(Graph *graph, int src)
{
    int V = graph->V;
    int *dist = (int *)malloc(V * sizeof(int));
    int *visited = (int *)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, visited, V);
        visited[u] = 1;

        Node *pCrawl = graph->array[u];
        while (pCrawl != NULL)
        {
            int v = pCrawl->vertex;
            int weight = pCrawl->weight;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
            pCrawl = pCrawl->next;
        }
    }

    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);

    free(dist);
    free(visited);
}

int main()
{
    int V, E;
    printf("Enter number of vertices: ");
    scanf("%d", &V);
    printf("Enter number of edges: ");
    scanf("%d", &E);

    Graph *graph = createGraph(V);

    printf("Enter edges in format: src dest weight\n");
    for (int i = 0; i < E; i++)
    {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        addEdge(graph, u, v, w);
    }

    int source;
    printf("Enter source vertex: ");
    scanf("%d", &source);

    dijkstra(graph, source);

    // Free graph memory
    for (int i = 0; i < V; i++)
    {
        Node *p = graph->array[i];
        while (p != NULL)
        {
            Node *temp = p;
            p = p->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);

    return 0;
}
