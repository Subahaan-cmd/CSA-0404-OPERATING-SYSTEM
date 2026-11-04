#include <stdio.h>

int main()
{
    int disk[50] = {0}, start, len, i, ok = 1;

    printf("Enter starting block and length: ");
    scanf("%d %d", &start, &len);

    for (i = start; i < start + len; i++)
        if (i >= 50 || disk[i]) ok = 0;            /* blocks must be free and contiguous */

    if (ok) {
        for (i = start; i < start + len; i++) {
            disk[i] = 1;
            printf("Block %d allocated\n", i);
        }
    } else
        printf("Blocks not free, file not allocated\n");
    return 0;
}