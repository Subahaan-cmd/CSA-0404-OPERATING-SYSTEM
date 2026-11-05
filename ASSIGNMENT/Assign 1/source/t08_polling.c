/* Task 8: polling versus interrupt (signal) driven detection of an abnormal event.
   A timer thread-less simulation: the "sensor" raises an event at a random time; we compare
   (a) polling every P ms, (b) waiting for a signal (SIGALRM), and report the CPU time used. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
static volatile sig_atomic_t flag = 0, got = 0;
static struct timespec t_event, t_seen;
static void on_alarm(int s) { (void)s; clock_gettime(CLOCK_MONOTONIC, &t_seen); got = 1; }
static double ms(struct timespec a, struct timespec b) { return (b.tv_sec - a.tv_sec) * 1e3 + (b.tv_nsec - a.tv_nsec) / 1e6; }
static double cpu_ms(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_utime.tv_sec * 1e3 + r.ru_utime.tv_usec / 1e3 + r.ru_stime.tv_sec * 1e3 + r.ru_stime.tv_usec / 1e3; }
static void sleep_ms(double m) { struct timespec t = {(time_t)(m / 1000), (long)((m - 1000 * (long)(m / 1000)) * 1e6)}; nanosleep(&t, NULL); }
int main(void)
{
    signal(SIGALRM, on_alarm);
    double periods[] = {50, 10, 1, 0};   /* 0 = busy polling */
    printf("%-18s %14s %14s\n", "method", "delay (ms)", "CPU used (ms)");
    for (int k = 0; k < 4; k++) {
        double P = periods[k], c0 = cpu_ms();
        struct timespec st; clock_gettime(CLOCK_MONOTONIC, &st);
        double event_at = 200.0 + 0.37 * (k + 1) * 10;      /* event at ~203.7 ms etc. */
        flag = 0;
        for (;;) {
            struct timespec n; clock_gettime(CLOCK_MONOTONIC, &n);
            double el = ms(st, n);
            if (el >= event_at && !flag) { flag = 1; t_event = n; t_event.tv_nsec -= (long)((el - event_at) * 1e6); }
            if (flag) { t_seen = n; break; }
            if (P > 0) sleep_ms(P);
        }
        double delay = ms(st, t_seen) - event_at;
        char nm[32]; if (P > 0) snprintf(nm, sizeof nm, "poll every %g ms", P); else snprintf(nm, sizeof nm, "busy polling");
        printf("%-18s %14.3f %14.3f\n", nm, delay, cpu_ms() - c0);
    }
    /* interrupt-driven: ask the kernel to deliver SIGALRM at 203 ms, and sleep (no CPU used) */
    double c0 = cpu_ms(); struct timespec st; clock_gettime(CLOCK_MONOTONIC, &st);
    struct itimerval it = {{0, 0}, {0, 203000}}; setitimer(ITIMER_REAL, &it, NULL);
    while (!got) pause();
    printf("%-18s %14.3f %14.3f\n", "interrupt (SIGALRM)", ms(st, t_seen) - 203.0, cpu_ms() - c0);
    return 0;
}
