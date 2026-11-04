
#include <stdio.h>

int main(void)
{
    int n, m, i, j, count = 0, found;
    int alloc[10][10], max[10][10], need[10][10];
    int avail[10], finish[10] = {0}, safe[10];

    printf("Enter number of processes (max 10): ");
    scanf("%d", &n);

    printf("Enter number of resource types (max 10): ");
    scanf("%d", &m);

    if (n < 1 || n > 10 || m < 1 || m > 10) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter Allocation Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Maximum Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available Resources:\n");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (alloc[i][j] < 0 || max[i][j] < alloc[i][j]) {
                printf("Invalid allocation or maximum value.\n");
                return 1;
            }
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    for (j = 0; j < m; j++) {
        if (avail[j] < 0) {
            printf("Invalid available resources.\n");
            return 1;
        }
    }

    while (count < n) {
        found = 0;

        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                for (j = 0; j < m; j++)
                    if (need[i][j] > avail[j])
                        break;

                if (j == m) {
                    for (j = 0; j < m; j++)
                        avail[j] += alloc[i][j];

                    safe[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found)
            break;
    }

    if (count == n) {
        printf("\nSystem is in a SAFE state.\n");
        printf("Safe Sequence: ");

        for (i = 0; i < n; i++) {
            printf("P%d", safe[i]);
            if (i < n - 1)
                printf(" -> ");
        }
        printf("\n");
    } else {
        printf("\nSystem is NOT in a safe state.\n");
    }

    return 0;
}
