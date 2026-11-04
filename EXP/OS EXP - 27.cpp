#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *d = opendir(argc > 1 ? argv[1] : ".");
    struct dirent *e;

    if (d == NULL) { perror("opendir"); return 1; }
    while ((e = readdir(d)) != NULL)
        printf("%s\n", e->d_name);
    closedir(d);
    return 0;
}