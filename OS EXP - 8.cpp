#include <stdio.h>

int main()
{
    int n, i, tq;
    int bt[10], rem[10];
    int wt[10] = {0}, tat[10];
    int time = 0;
    int done;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);

        rem[i] = bt[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &tq);

    /* Round Robin Scheduling */
    do
    {
        done = 1;

        for (i = 0; i < n; i++)
        {
            if (rem[i] > 0)
            {
                done = 0;

                if (rem[i] > tq)
                {
                    time = time + tq;
                    rem[i] = rem[i] - tq;
                }
                else
                {
                    time = time + rem[i];

                    wt[i] = time - bt[i];
                    rem[i] = 0;
                }
            }
        }

    } while (done == 0);

    /* Calculate Turnaround Time */
    for (i = 0; i < n; i++)
    {
        tat[i] = wt[i] + bt[i];
    }

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\n",
               i + 1, bt[i], wt[i], tat[i]);
    }

    return 0;
}