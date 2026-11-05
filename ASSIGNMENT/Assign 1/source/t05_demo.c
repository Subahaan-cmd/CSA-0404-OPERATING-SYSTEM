/* Task 5: a tiny program whose system calls are easy to follow in the trace. */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    char buf[100];
    int fd = open("/etc/hostname", O_RDONLY);       /* file management  */
    ssize_t n = read(fd, buf, sizeof buf);          /* the read() call  */
    write(1, buf, (size_t)n);                       /* device (stdout)  */
    close(fd);
    return 0;
}
