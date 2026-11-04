#include <stdio.h>
#include <sys/stat.h>
int main(int argc, char *argv[]) {
    struct stat s;
    if (argc < 2 || stat(argv[1], &s)) { perror("stat"); return 1; }
    printf("Owner : %c%c%c\n", s.st_mode & S_IRUSR ? 'r' : '-', s.st_mode & S_IWUSR ? 'w' : '-', s.st_mode & S_IXUSR ? 'x' : '-');
    printf("Group : %c%c%c\n", s.st_mode & S_IRGRP ? 'r' : '-', s.st_mode & S_IWGRP ? 'w' : '-', s.st_mode & S_IXGRP ? 'x' : '-');
    printf("Others: %c%c%c\n", s.st_mode & S_IROTH ? 'r' : '-', s.st_mode & S_IWOTH ? 'w' : '-', s.st_mode & S_IXOTH ? 'x' : '-');
}