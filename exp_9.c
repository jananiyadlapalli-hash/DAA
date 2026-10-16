// Experiment 9: 0/1 Knapsack - Greedy vs Dynamic Programming on the same dataset
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id, w, v;
    double ratio;
} Item;

int cmp_ratio(const void *a, const void *b) {
    double x = ((Item *)a)->ratio, y = ((Item *)b)->ratio;
    return (y > x) - (y < x);   // descending
}

int main() {
    int n, W;
    printf("Enter number of items: ");
    scanf("%d", &n);
    Item *it = malloc(n * sizeof(Item));
    printf("Enter weight and value of each item:\n");
    for (int i = 0; i < n; i++) {
        it[i].id = i + 1;
        printf("Item %d: ", i + 1);
        scanf("%d %d", &it[i].w, &it[i].v);
        it[i].ratio = (double)it[i].v / it[i].w;
    }
    printf("Enter knapsack capacity: ");
    scanf("%d", &W);

    // ---------- Greedy (by value/weight ratio) ----------
    Item *g = malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) g[i] = it[i];
    qsort(g, n, sizeof(Item), cmp_ratio);

    int gw = 0, gv = 0;
    printf("\n--- Greedy (highest value/weight first) ---\nItems taken:");
    for (int i = 0; i < n; i++) {
        if (gw + g[i].w <= W) {
            gw += g[i].w;
            gv += g[i].v;
            printf(" %d", g[i].id);
        }
    }
    printf("\nTotal weight = %d, Total value = %d\n", gw, gv);

    // ---------- Dynamic Programming ----------
    int **K = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) K[i] = calloc(W + 1, sizeof(int));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            K[i][w] = K[i - 1][w];
            if (it[i - 1].w <= w && K[i - 1][w - it[i - 1].w] + it[i - 1].v > K[i][w])
                K[i][w] = K[i - 1][w - it[i - 1].w] + it[i - 1].v;
        }
    }

    printf("\n--- Dynamic Programming ---\nItems taken:");
    int w = W, dw = 0;
    for (int i = n; i >= 1; i--) {
        if (K[i][w] != K[i - 1][w]) {
            printf(" %d", it[i - 1].id);
            w -= it[i - 1].w;
            dw += it[i - 1].w;
        }
    }
    printf("\nTotal weight = %d, Total value = %d\n", dw, K[n][W]);

    // ---------- Comparison ----------
    printf("\n--- Comparison ---\n");
    printf("Greedy value : %d  (O(n log n), not always optimal)\n", gv);
    printf("DP value     : %d  (O(n*W), always optimal)\n", K[n][W]);
    if (K[n][W] > gv)
        printf("Greedy missed %d value (%.1f%% worse).\n", K[n][W] - gv, 100.0 * (K[n][W] - gv) / K[n][W]);
    else
        printf("Greedy matched the optimal answer on this dataset.\n");

    for (int i = 0; i <= n; i++) free(K[i]);
    free(K);
    free(it);
    free(g);
    return 0;
}