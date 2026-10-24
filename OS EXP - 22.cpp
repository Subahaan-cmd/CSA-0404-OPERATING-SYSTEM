
#include <stdio.h>

int main() {
    int b[20], p[20], nb, np, i, j, k;

    printf("Number of blocks: ");
    scanf("%d", &nb);
    if (nb < 1 || nb > 20) return 1;

    for (i = 0; i < nb; i++) {
        printf("Block %d size: ", i + 1);
        scanf("%d", &b[i]);
    }

    printf("Number of processes: ");
    scanf("%d", &np);
    if (np < 1 || np > 20) return 1;

    for (i = 0; i < np; i++) {
        printf("Process %d size: ", i + 1);
        scanf("%d", &p[i]);
    }

    for (i = 0; i < np; i++) {
        k = -1;

        for (j = 0; j < nb; j++) {
            if (b[j] >= p[i] &&
                (k == -1 || b[j] < b[k]))
                k = j;
        }

        if (k == -1) {
            printf("P%d not allocated\n", i + 1);
        } else {
            printf("P%d allocated to Block %d\n",
                   i + 1, k + 1);
            b[k] -= p[i];
        }
    }

    return 0;
}
