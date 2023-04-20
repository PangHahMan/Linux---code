#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TASK_NUM 10

typedef void (*func_t)(); // 函数指针
func_t other_task[TASK_NUM] = {NULL};

// 预设一批任务
void sync_disk()
{
    printf("这是一个刷新数据的任务!\n");
}

void sync_log()
{
    printf("这是一个同步日志的任务!\n");
}

void net_send()
{
    printf("这是一个进行网络发送的任务!\n");
}

// 加载任务函数指针到任务列表中
int LoadTask(func_t func)
{
    int i = 0;
    for (; i < TASK_NUM; i++)
    {
        if (other_task[i] == NULL)   //为NULL ，直接推虎
            break;
    }
    if (i == TASK_NUM)   //i超过，return -1
        return -1;
    else
        other_task[i] = func;   //赋值
    return 0;
}

// 初始化任务列表
void InitTask()
{
    for (int i = 0; i < TASK_NUM; i++)
        other_task[i] = NULL;
    LoadTask(sync_disk);
    LoadTask(sync_log);
    LoadTask(net_send);
}

// 执行任务列表中的所有任务
void RunTask()
{
    for (int i = 0; i < TASK_NUM; i++)
    {
        if (other_task[i] == NULL)
            continue;
        other_task[i]();
    }
}

int main()
{
    // 创建子进程
    pid_t id = fork();
    if (id == 0) // 子进程执行的代码
    {
        //子进程
        int cnt = 5;
        while (cnt)
        {
            printf("我是子进程，我还活着呢，我还有%dS,pid:%d,ppid:%d\n", cnt--, getpid(), getppid());
            sleep(1);
        }
        if(1==1) exit(0);
        exit(107); // 子进程退出并返回107
    }

    // 父进程执行的代码
    InitTask(); // 初始化任务列表
    while (1)
    {
        int status = 0;
        pid_t ret_id = waitpid(id, &status, WNOHANG); // 检测子进程状态
        if (ret_id < 0)
        {
            printf("waitpid error!\n");
            exit(1);
        }
        else if (ret_id == 0) // 子进程还在运行
        {
            RunTask(); // 执行任务列表中的所有任务
            sleep(1);
            continue;
        }
        else // 子进程已退出
        {
            if (WIFEXITED(status))
            {
                printf("wait success,child exit code:%d\n", WEXITSTATUS(status));
            }
            else
            {
                printf("wait success,child exit signal:%d\n", status & 0x7F);
            }
            break;
            //printf("我是父进程，等待子进程成功,pid:%d,ppid:%d,ret_id:%d,child exit code:%d,child exit singal:%d\n", getpid(), getppid(), ret_id, (status >> 8) & 0xFF, status & 0x7F);
        }
    }


    return 0;
}