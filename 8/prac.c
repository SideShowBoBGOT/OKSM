#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main()
{
    char *command[3] = {"/bin/ls", "-l", 0};
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(fd[1], 1);
        execvp(command[0], command);
    }
    wait(0);
    return 0;
}