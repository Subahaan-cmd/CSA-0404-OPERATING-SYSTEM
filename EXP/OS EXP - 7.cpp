#include <stdio.h>

int main()
{
    int at[10], bt[10], pr[10];
    int n, i, j, temp;
    int time = 0, over = 0;
    int sum_wait = 0, sum_turnaround = 0;
    int start;
    float avgwait, avgturn;

    printf("Enter the number of processes\n");
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("\nEnter arrival time of P%d: ", i + 1);
        scanf("%d", &at[i]);

        printf("Enter burst time of P%d: ", i + 1);
        scanf("%d", &bt[i]);

        printf("Enter priority of P%d: ", i + 1);
        scanf("%d", &pr[i]);
    }

    /* Sort according to arrival time */
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (at[i] > at[j])
            {
                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                temp = pr[i];
                pr[i] = pr[j];
                pr[j] = temp;
            }
        }
    }

    printf("\n\nProcess | Arrival Time | Burst Time | Start Time | End Time | Waiting Time | Turnaround Time\n");

    while (over < n)
    {
        /* Select the highest priority process among arrived processes */
        for (i = over; i < n; i++)
        {
            if (at[i] <= time)
            {
                for (j = i + 1; j < n; j++)
                {
                    if (at[j] <= time && pr[i] > pr[j])
                    {
                        temp = at[i];
                        at[i] = at[j];
                        at[j] = temp;

                        temp = bt[i];
                        bt[i] = bt[j];
                        bt[j] = temp;

                        temp = pr[i];
                        pr[i] = pr[j];
                        pr[j] = temp;
                    }
                }
            }
        }

        /* If no process has arrived, move time forward */
        if (time < at[over])
        {
            time = at[over];
        }

        start = time;
        time = time + bt[over];

        printf("P[%d]\t | %d\t\t | %d\t\t | %d\t\t | %d\t | %d\t\t | %d\n",
               pr[over],
               at[over],
               bt[over],
               start,
               time,
               start - at[over],
               time - at[over]);

        sum_wait = sum_wait + (start - at[over]);
        sum_turnaround = sum_turnaround + (time - at[over]);

        over++;
    }

    avgwait = (float)sum_wait / n;
    avgturn = (float)sum_turnaround / n;

    printf("\nAverage waiting time is %f\n", avgwait);
    printf("Average turnaround time is %f\n", avgturn);

    return 0;
}