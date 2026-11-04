#include <stdio.h>
#include <pthread.h>

static char msg[] = "Thread exited";

void *work(void *arg)
{
    printf("Inside thread\n");
    pthread_exit(msg);                             
}

int main()
{
    pthread_t t;
    void *ret;

    pthread_create(&t, NULL, work, NULL);          
    pthread_join(t, &ret);                            
    printf("Returned: %s\n", (char *)ret);

    if (pthread_equal(t, pthread_self()))             /* (iii) equal */
        printf("Same thread\n");
    else
        printf("Different threads\n");
    return 0;
}