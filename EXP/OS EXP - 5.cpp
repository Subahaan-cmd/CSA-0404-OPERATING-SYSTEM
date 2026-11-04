#include <stdio.h>

int main()
{
    int n, i, j;
    int process[20], burst[20], priority[20];
    int waiting[20], turnaround[20];
    int temp;
    float avgWaiting = 0, avgTurnaround = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter burst time and priority:\n");

    for (i = 0; i < n; i++)
    {
        process[i] = i + 1;

        printf("\nP%d burst time: ", i + 1);
        scanf("%d", &burst[i]);

        printf("P%d priority: ", i + 1);
        scanf("%d", &priority[i]);
    }

    /* Sort according to priority */
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (priority[i] > priority[j])
            {
                temp = priority[i];
                priority[i] = priority[j];
                priority[j] = temp;

                temp = burst[i];
                burst[i] = burst[j];
                burst[j] = temp;

                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    /* Calculate waiting time */
    waiting[0] = 0;

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

    printf("\nProcess\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n",
               process[i],
               burst[i],
               priority[i],
               waiting[i],
               turnaround[i]);
    }

    printf("\nAverage Waiting Time = %.2f", avgWaiting);
    printf("\nAverage Turnaround Time = %.2f\n", avgTurnaround);

    return 0;
}