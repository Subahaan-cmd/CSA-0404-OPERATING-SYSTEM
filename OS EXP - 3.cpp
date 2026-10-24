#include <stdio.h>

int main()
{
    int n, i;
    int burst[20], waiting[20], turnaround[20];
    float avgWaiting = 0, avgTurnaround = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &burst[i]);
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
    }

    /* Calculate averages */
    for (i = 0; i < n; i++)
    {
        avgWaiting += waiting[i];
        avgTurnaround += turnaround[i];
    }

    avgWaiting = avgWaiting / n;
    avgTurnaround = avgTurnaround / n;

    /* Display results */
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\n",
               i + 1, burst[i], waiting[i], turnaround[i]);
    }

    printf("\nAverage Waiting Time = %.2f", avgWaiting);
    printf("\nAverage Turnaround Time = %.2f\n", avgTurnaround);

    return 0;
}