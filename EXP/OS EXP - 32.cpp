#include <stdio.h>

int main()
{
    int ref[] = {7,0,1,2,0,3,0,4,2,3,0,3,2}, n = 13;
    int f = 3, fr[3] = {-1, -1, -1}, last[3] = {0, 0, 0}, faults = 0;

    for (int i = 0; i < n; i++) {
        int idx = -1;
        for (int j = 0; j < f; j++)
            if (fr[j] == ref[i]) idx = j;

        if (idx != -1) last[idx] = i + 1;          /* hit: update last-used time */
        else {
            int v = 0;
            for (int j = 1; j < f; j++)
                if (last[j] < last[v]) v = j;      /* least recently used */
            fr[v] = ref[i];
            last[v] = i + 1;
            faults++;
        }
        printf("%d: %d %d %d %s\n", ref[i], fr[0], fr[1], fr[2], idx == -1 ? "(fault)" : "");
    }
    printf("Total page faults = %d\n", faults);
    return 0;
}