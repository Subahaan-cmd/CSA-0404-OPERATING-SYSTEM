#include <stdio.h>

int main()
{
    int n, i, j;
    int burst[20], process[20];
    int waiting[20], turnaround[20];
    int temp;
    float avgWaiting = 0, avgTurnaround = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");

    for (i = 0; i < n; i++)
    {
        process[i] = i + 1;
        printf("P%d: ", i + 1);
        scanf("%d", &burst[i]);
    }

    /* Sort processes according to burst time */
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (burst[i] > burst[j])
            {
                temp = burst[i];
                burst[i] = burst[j];
                burst[j] = temp;

                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    /* First process has zero waiting time */
    waiting[0] = 0;

    /* Calculate waiting time */
    for (i = 1; i < n; i++)
    {
        waiting[i] = waiting[i - 1] + burst[i - 1];
    }

    /* Calculate turnaround time */
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
               process[i], burst[i],
               waiting[i], turnaround[i]);
    }

    printf("\nAverage Waiting Time = %.2f", avgWaiting);
    printf("\nAverage Turnaround Time = %.2f\n", avgTurnaround);

    return 0;
}