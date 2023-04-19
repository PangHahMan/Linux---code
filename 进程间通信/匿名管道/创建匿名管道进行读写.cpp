#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
using namespace std;

int main()
{
    // 创建管道
    int pipefd[2] = {0};
    int fd = pipe(pipefd);

    // 2.创建子进程
    pid_t id = fork();

    if (id == 0)
    {
        // 子进程读
        close(pipefd[1]);
        // 开始通信
        char buffer[1024];
        while (true)
        {
            int n = read(pipefd[0], buffer, sizeof(buffer));
            if (n > 0)
            {
                buffer[n] = '\0';
                cout << "我是子进程,father give me message:" << buffer << endl;
            }
        }
    }

    // 父进程
    close(pipefd[0]);
    char buffer[1024];
    string namestr = "i am father";
    
    while (true)
    {
        snprintf(buffer, sizeof(buffer), "%s", namestr.c_str());
        write(pipefd[1], buffer, sizeof(buffer));
        sleep(1);
    }
    return 0;
}