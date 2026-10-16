// Experiment 7-8: Dijkstra vs Bellman-Ford (Single Source Shortest Path)
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF (INT_MAX / 2)

typedef struct {
    int u, v, w;
} Edge;

int V, E;
Edge *edges;
int **adj;   // adjacency matrix for Dijkstra (0 = no edge)

// Dijkstra, O(V^2) with adjacency matrix. Needs non-negative weights.
void dijkstra(int src, int *dist) {
    int *done = calloc(V, sizeof(int));
    for (int i = 0; i < V; i++) dist[i] = INF;
    dist[src] = 0;
    for (int c = 0; c < V - 1; c++) {
        int u = -1;
        for (int i = 0; i < V; i++)
            if (!done[i] && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1 || dist[u] == INF) break;
        done[u] = 1;
        for (int v = 0; v < V; v++)
            if (adj[u][v] && !done[v] && dist[u] + adj[u][v] < dist[v])
                dist[v] = dist[u] + adj[u][v];
    }
    free(done);
}

// Bellman-Ford, O(V*E). Works with negative weights. Returns 0 if negative cycle found.
int bellman_ford(int src, int *dist) {
    for (int i = 0; i < V; i++) dist[i] = INF;
    dist[src] = 0;
    for (int i = 1; i < V; i++) {
        int changed = 0;
        for (int j = 0; j < E; j++) {
            int u = edges[j].u, v = edges[j].v, w = edges[j].w;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                changed = 1;
            }
        }
        if (!changed) break;
    }
    for (int j = 0; j < E; j++)
        if (dist[edges[j].u] != INF && dist[edges[j].u] + edges[j].w < dist[edges[j].v])
            return 0;
    return 1;
}

void build_matrix() {
    adj = malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) adj[i] = calloc(V, sizeof(int));
    for (int j = 0; j < E; j++) adj[edges[j].u][edges[j].v] = edges[j].w;
}

void free_matrix() {
    for (int i = 0; i < V; i++) free(adj[i]);
    free(adj);
}

void print_dist(const char *name, int *dist) {
    printf("%s distances:\n", name);
    for (int i = 0; i < V; i++) {
        if (dist[i] >= INF) printf("  %d : INF\n", i);
        else printf("  %d : %d\n", i, dist[i]);
    }
}

int main() {
    int choice, src = 0;
    printf("1. Enter graph manually\n2. Random graph benchmark\nChoice: ");
    scanf("%d", &choice);

    int reps = 1;
    if (choice == 1) {
        printf("Enter vertices and edges: ");
        scanf("%d %d", &V, &E);
        edges = malloc(E * sizeof(Edge));
        printf("Enter each edge as: u v weight (vertices numbered from 0)\n");
        for (int i = 0; i < E; i++) scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);
        printf("Enter source vertex: ");
        scanf("%d", &src);
        reps = 1000;
    } else {
        printf("Enter vertices and edges (edges >= vertices-1): ");
        scanf("%d %d", &V, &E);
        if (E < V - 1 || (long long)E > (long long)V * (V - 1)) {
            printf("Invalid edge count.\n");
            return 1;
        }
        edges = malloc(E * sizeof(Edge));
        srand(7);
        // temporary matrix to avoid duplicate edges
        char *used = calloc((size_t)V * V, 1);
        int cnt = 0;
        for (int i = 0; i + 1 < V; i++) {   // chain so every vertex is reachable
            edges[cnt++] = (Edge){i, i + 1, rand() % 100 + 1};
            used[(size_t)i * V + i + 1] = 1;
        }
        while (cnt < E) {
            int u = rand() % V, v = rand() % V;
            if (u == v || used[(size_t)u * V + v]) continue;
            used[(size_t)u * V + v] = 1;
            edges[cnt++] = (Edge){u, v, rand() % 100 + 1};
        }
        free(used);
        reps = V <= 200 ? 100 : 1;
    }

    build_matrix();
    int *d1 = malloc(V * sizeof(int));
    int *d2 = malloc(V * sizeof(int));

    clock_t t0 = clock();
    for (int r = 0; r < reps; r++) dijkstra(src, d1);
    double t_dij = (double)(clock() - t0) / CLOCKS_PER_SEC / reps;

    int ok = 1;
    t0 = clock();
    for (int r = 0; r < reps; r++) ok = bellman_ford(src, d2);
    double t_bf = (double)(clock() - t0) / CLOCKS_PER_SEC / reps;

    if (V <= 10) {
        print_dist("Dijkstra", d1);
        if (ok) print_dist("Bellman-Ford", d2);
    }
    if (!ok) printf("Bellman-Ford: negative weight cycle detected!\n");

    int same = 1;
    for (int i = 0; i < V; i++) if (d1[i] != d2[i]) same = 0;

    printf("\nVertices = %d, Edges = %d\n", V, E);
    printf("Dijkstra     : %.8f s per run  (O(V^2))\n", t_dij);
    printf("Bellman-Ford : %.8f s per run  (O(V*E))\n", t_bf);
    printf("Both give the same distances: %s\n", same ? "yes" : "no (negative edges break Dijkstra)");

    free(d1);
    free(d2);
    free_matrix();
    free(edges);
    return 0;
}