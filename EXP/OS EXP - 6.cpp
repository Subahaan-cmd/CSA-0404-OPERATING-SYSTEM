#include <stdio.h>

int main()
{
    int n, i;
    int burst[20], remaining[20];
    int waiting[20], turnaround[20];
    int quantum;
    int time = 0;
    int completed = 0;
    float avgWaiting = 0, avgTurnaround = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &burst[i]);
        remaining[i] = burst[i];
        waiting[i] = 0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    /* Round Robin Scheduling */
    while (completed < n)
    {
        for (i = 0; i < n; i++)
        {
            if (remaining[i] > 0)
            {
                if (remaining[i] > quantum)
                {
                    time = time + quantum;
                    remaining[i] = remaining[i] - quantum;
                }
                else
                {
                    time = time + remaining[i];
                    waiting[i] = time - burst[i];
                    remaining[i] = 0;
                    completed++;
                }
            }
        }
    }

    /* Calculate Turnaround Time */
    for (i = 0; i < n; i++)
    {
        turnaround[i] = waiting[i] + burst[i];

        avgWaiting = avgWaiting + waiting[i];
        avgTurnaround = avgTurnaround + turnaround[i];
    }

    avgWaiting = avgWaiting / n;
    avgTurnaround = avgTurnaround / n;

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\n",
               i + 1,
               burst[i],
               waiting[i],
               turnaround[i]);
    }

    printf("\nAverage Waiting Time = %.2f", avgWaiting);
    printf("\nAverage Turnaround Time = %.2f\n", avgTurnaround);

    return 0;
}