#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t id;
/*  副本

void handler(int signo)
{
    char msg[] = "捕捉到一个信号:17,who:";
    char pid_str[16];
    sprintf(pid_str, "%d", getpid());
    strcat(msg, pid_str);
    strcat(msg, "\n");
    write(STDOUT_FILENO, msg, strlen(msg));
    sleep(5);
    pid_t res = waitpid(-1, NULL, 0);
    if (res > 0)
    {
        printf("wait success,res:%d,id:%d\n", res, id);
    }
}

int main()
{
    signal(SIGCHLD, handler);
    id = fork();

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

    //如果你的父进程没有事干，你还是用以前的方法
    //如果你的父进程很忙，而且不退出，可以选择这种信号方法waitpid
    while (1)
    {
        sleep(1);
    }

    return 0;
}
*/

void handler(int signo)
{
    char msg[] = "捕捉到一个信号:17,who:";
    char pid_str[16];
    sprintf(pid_str, "%d", getpid());
    strcat(msg, pid_str);
    strcat(msg, "\n");
    write(STDOUT_FILENO, msg, strlen(msg));

    sleep(5);

    while (1)
    {
        pid_t res = waitpid(-1, NULL, 0);
        if (res > 0)
        {
            printf("wait success,res:%d,id:%d\n", res, id);
        }
        else
        {
            break; // 没有子进程了就退出
        }
    }
    printf("handler done...\n");
}

int main()
{
    signal(SIGCHLD, handler);
    for (int i = 1; i <= 10; i++)
    {
        id = fork();
        if (id == 0)
        {
            int cnt = 5;
            while (cnt)
            {
                printf("我是子进程,我的pid:%d,ppid:%d\n", getpid(), getppid());
                sleep(1);
                cnt--;
            }
            exit(1);
        }
    }
    // 如果你的父进程没有事干，你还是用以前的方法
    // 如果你的父进程很忙，而且不退出，可以选择这种信号方法waitpid
    while (1)
    {
        sleep(1);
    }

    return 0;
}