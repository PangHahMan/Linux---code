#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include "task.hpp"
using namespace std;
// 父进程利用一批管道控制子进程
const int num = 5; // num用于下面的循环创建子进程，5个子进程
Task t;            // 全局对象
// 子进程的结构，保存了pid和文件描述符fd，方面父进程来指定访问哪个子进程
struct EndPoint
{
    EndPoint(int id, int fd)
        : child_id(id),
          write_fd(fd) {}
    ~EndPoint(){};
    pid_t child_id; // 子进程的pid
    int write_fd;   // 写端的文件描述符
};

// 子进程要执行的方法
void WaitCommand()
{
    while (true)
    {
        int command = 0;
        // read的返回值表示读取的字节数
        // 将从管道的读端读取的数据，放到command缓冲区中
        int n = read(0, &command, sizeof(int));
        if (n == sizeof(int))
        {
            // n没问题就调用函数
            t.Execute(command);
        }
        else if (n == 0)
        {
            break;
        }
        else
        {
            break;
        }
    }
}

void CreatProcess(vector<EndPoint> *end_points)
{
    // 创建多个进程
    for (int i = 0; i < num; i++)
    {
        // 1.1创建管道 - 1个读 一个写
        int pipefd[2] = {0};
        int n = pipe(pipefd);
        assert(n == 0); // n必须等于0
        (void)n;
        // 1.2创建进程
        pid_t id = fork();
        assert(id != -1); // id必须不为-1
        // 一定是子进程
        if (id == 0)
        {
            // 1.3关闭不需要的fd
            close(pipefd[1]);
            // 我们期望，所有的子进程读取指令的时候，都从标准输入读取
            // 1.3.1 输入重定向，可以不做  管道的读端改成输入0
            dup2(pipefd[0], 0);
            // 1.3.2 子进程开始等待获取命令
            WaitCommand();
            close(pipefd[0]);
            exit(0);
        }

        // 一定是父进程
        // 1.3关闭不需要的fd
        close(pipefd[0]);

        // 1.4 将新的子进程和他的管道写端，构建对象
        end_points->push_back(EndPoint(id, pipefd[1]));
    }
}

int main()
{
    // 1.先进行构建控制结构,父进程写入，子进程读取
    vector<EndPoint> end_points;
    // 创建了多个子进程，子进程的pid和文件描述符都保存在end_points对象中
    CreatProcess(&end_points);
    // 2. 我们得到了什么?  end_points
    // 2.1 我们可以写成自动化的，也可以搞成交互式的
    int num = 0;
    // 父进程
    while (true)
    {
        // 1.选择任务
        int command = COMMAND_LOG;

        // 2.选择进程，随机
        int index = rand() % end_points.size();
        // 3.下发任务 - 向子进程创建的管道文件描述中写入command，
        // 等待子进程read出command的值，然后通过对应的command调用Task函数
        write(end_points[index].write_fd, &command, sizeof(command));
        sleep(1);
    }
    return 0;
}