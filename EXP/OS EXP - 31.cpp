#include <stdio.h>

int main()
{
    int ref[] = {7,0,1,2,0,3,0,4,2,3,0,3,2}, n = 13;
    int f = 3, fr[3] = {-1, -1, -1}, next = 0, faults = 0;

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < f; j++)
            if (fr[j] == ref[i]) hit = 1;
        if (!hit) {
            fr[next] = ref[i];                 /* replace oldest page */
            next = (next + 1) % f;
            faults++;
        }
        printf("%d: %d %d %d %s\n", ref[i], fr[0], fr[1], fr[2], hit ? "" : "(fault)");
    }
    printf("Total page faults = %d\n", faults);
    return 0;
}