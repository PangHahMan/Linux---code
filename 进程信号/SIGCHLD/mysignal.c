#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signo)
{
    char msg[] = "捕捉到一个信号:17,who:";
    char pid_str[16];
    sprintf(pid_str, "%d", getpid());
    strcat(msg, pid_str);
    strcat(msg, "\n");
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main()
{
    pid_t id = fork();
    signal(SIGCHLD, handler);

    if (id == 0)
    {
        int cnt = 5;
        while (cnt)
        {
            printf("我是子进程,我是pid:%d ppid:%d\n", getpid(), getppid());
            sleep(1);
            cnt--;
        }

        exit(1);
    }

    while (1)
    {
        sleep(1);
    }

    return 0;
}