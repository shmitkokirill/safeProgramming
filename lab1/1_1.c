#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int pid;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        execlp("df", "df", NULL);
        perror("df");
        exit(1);
    }
    wait(NULL);
    return 0;
}