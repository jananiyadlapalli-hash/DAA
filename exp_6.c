// Experiment 6: Matrix Chain Multiplication (Dynamic Programming)
// Shows how parenthesization changes both the operation count and the real running time.
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX 101

long long m[MAX][MAX];   // m[i][j] = min scalar multiplications for A_i..A_j
int s[MAX][MAX];         // s[i][j] = best split point k
int p[MAX];              // A_i has dimensions p[i-1] x p[i]
double *mat[MAX];        // actual matrices (for timing)

void print_paren(int i, int j) {
    if (i == j) {
        printf("A%d", i);
        return;
    }
    printf("(");
    print_paren(i, s[i][j]);
    printf(" ");
    print_paren(s[i][j] + 1, j);
    printf(")");
}

// C = A (r x k) * B (k x c)
double *mul(double *A, int r, int k, double *B, int c) {
    double *C = calloc((size_t)r * c, sizeof(double));
    for (int i = 0; i < r; i++)
        for (int t = 0; t < k; t++) {
            double a = A[(size_t)i * k + t];
            for (int j = 0; j < c; j++)
                C[(size_t)i * c + j] += a * B[(size_t)t * c + j];
        }
    return C;
}

// Multiply A_i..A_j following the optimal split table
double *chain_optimal(int i, int j) {
    if (i == j) return mat[i];
    int k = s[i][j];
    double *X = chain_optimal(i, k);
    double *Y = chain_optimal(k + 1, j);
    double *Z = mul(X, p[i - 1], p[k], Y, p[j]);
    if (i != k) free(X);
    if (k + 1 != j) free(Y);
    return Z;
}

// Multiply strictly left to right: ((A1 A2) A3) ...
double *chain_left_to_right(int n) {
    double *res = mat[1];
    for (int i = 2; i <= n; i++) {
        double *next = mul(res, p[0], p[i - 1], mat[i], p[i]);
        if (i > 2) free(res);
        res = next;
    }
    return res;
}

int main() {
    int n;
    printf("Enter number of matrices (max %d): ", MAX - 1);
    scanf("%d", &n);
    if (n < 2 || n >= MAX) {
        printf("Need between 2 and %d matrices.\n", MAX - 1);
        return 1;
    }
    printf("Enter %d dimensions p0..p%d (matrix Ai is p[i-1] x p[i]): ", n + 1, n);
    for (int i = 0; i <= n; i++) scanf("%d", &p[i]);

    // ---- Dynamic programming ----
    for (int i = 1; i <= n; i++) m[i][i] = 0;
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            m[i][j] = LLONG_MAX;
            for (int k = i; k < j; k++) {
                long long q = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    // Cost of naive left-to-right order
    long long naive = 0;
    for (int i = 2; i <= n; i++) naive += (long long)p[0] * p[i - 1] * p[i];

    printf("\nOptimal parenthesization: ");
    print_paren(1, n);
    printf("\nMinimum multiplications (DP)  : %lld\n", m[1][n]);
    printf("Left-to-right multiplications : %lld\n", naive);
    if (m[1][n] > 0)
        printf("Left-to-right needs %.2fx more work\n", (double)naive / m[1][n]);

    // ---- Real timing ----
    int run;
    printf("\nRun actual multiplication to compare time? (1 = yes, 0 = no): ");
    scanf("%d", &run);
    if (run) {
        srand(42);
        for (int i = 1; i <= n; i++) {
            size_t sz = (size_t)p[i - 1] * p[i];
            mat[i] = malloc(sz * sizeof(double));
            for (size_t t = 0; t < sz; t++) mat[i][t] = (double)rand() / RAND_MAX;
        }

        clock_t t0 = clock();
        double *R1 = chain_optimal(1, n);
        double t_opt = (double)(clock() - t0) / CLOCKS_PER_SEC;

        t0 = clock();
        double *R2 = chain_left_to_right(n);
        double t_naive = (double)(clock() - t0) / CLOCKS_PER_SEC;

        printf("Time with optimal order     : %.4f s\n", t_opt);
        printf("Time with left-to-right order: %.4f s\n", t_naive);

        free(R1);
        free(R2);
        for (int i = 1; i <= n; i++) free(mat[i]);
    }
    return 0;
}