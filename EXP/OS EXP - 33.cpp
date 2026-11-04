#include <stdio.h>

int main()
{
    int ref[] = {7,0,1,2,0,3,0,4,2,3,0,3,2}, n = 13;
    int f = 3, fr[3] = {-1, -1, -1}, faults = 0;

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < f; j++)
            if (fr[j] == ref[i]) hit = 1;

        if (!hit) {
            int v = -1, far = -1;
            for (int j = 0; j < f; j++) {
                if (fr[j] == -1) { v = j; break; }       /* empty frame first */
                int k;
                for (k = i + 1; k < n; k++)
                    if (ref[k] == fr[j]) break;          /* next use of this page */
                if (k > far) { far = k; v = j; }         /* used farthest in future */
            }
            fr[v] = ref[i];
            faults++;
        }
        printf("%d: %d %d %d %s\n", ref[i], fr[0], fr[1], fr[2], hit ? "" : "(fault)");
    }
    printf("Total page faults = %d\n", faults);
    return 0;
}