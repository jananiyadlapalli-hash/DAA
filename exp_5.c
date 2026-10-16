// Experiment 5: Activity Selection (Greedy)
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id, start, finish;
} Activity;

int cmp(const void *a, const void *b) {
    return ((Activity *)a)->finish - ((Activity *)b)->finish;
}

int main() {
    int n;
    printf("Enter number of activities: ");
    scanf("%d", &n);

    Activity *a = malloc(n * sizeof(Activity));
    for (int i = 0; i < n; i++) {
        a[i].id = i + 1;
        printf("Activity %d - start finish: ", i + 1);
        scanf("%d %d", &a[i].start, &a[i].finish);
    }

    // Greedy step 1: sort by finish time
    qsort(a, n, sizeof(Activity), cmp);

    // Greedy step 2: always pick the next activity that starts after the last chosen one ends
    printf("\nSelected activities:\n");
    int last = 0, count = 1;
    printf("Activity %d (%d - %d)\n", a[0].id, a[0].start, a[0].finish);
    for (int i = 1; i < n; i++) {
        if (a[i].start >= a[last].finish) {
            printf("Activity %d (%d - %d)\n", a[i].id, a[i].start, a[i].finish);
            last = i;
            count++;
        }
    }
    printf("Total activities selected: %d\n", count);

    free(a);
    return 0;
}