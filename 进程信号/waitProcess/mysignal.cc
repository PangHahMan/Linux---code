#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
using namespace std;

pid_t id;
void waitProcess(int signo)
{
    cout << "捕捉到一个信号" << signo << "who:" << getpid() << endl;

    sleep(5);

    while (1)
    {
        pid_t res = waitpid(-1, nullptr, WNOHANG); // 非阻塞等待
        if (res > 0)
            cout << "wait success,res:" << res << ",id:" << id << endl;
        else
            break;
    }
    cout << "handler done..." << endl;
}

int main()
{
    // Linux下可以用signal函数直接自动清理掉fork的子进程,参数带上SIG_ING,就不用waitpid了
    signal(SIGCHLD, SIG_IGN);
    // signal(SIGCHLD, waitProcess);

    for (int i = 1; i <= 10; i++)
    {
        id = fork();
        if (id == 0)
        {
            int cnt = 5;
            while (cnt)
            {
                cout << "我是子进程,我的pid:" << getpid() << ",ppid:" << getppid() << endl;
                sleep(1);
                cnt--;
            }
            exit(1);
        }
    }

    while (1)
    {
        sleep(1);
    }
    return 0;
}

// Linux下可以用signal函数直接自动清理掉fork的子进程
