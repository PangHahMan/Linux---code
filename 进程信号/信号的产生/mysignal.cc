#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

void handler(int signo)
{
    cout << "我们的进程确实收到了:" << signo << "信号导致崩溃的" << endl;
}

// a/0会崩溃    收到8信号
// void test1()
// {
//     signal(SIGFPE, handler);
//     int a = 10;
//     a /= 0; // 报错，收到8号信号
//     cout << "div zero...here" << endl;
// }

// 野指针问题会出现异常，发送SIGSEGV信号  11
void test2()
{
    signal(SIGSEGV, handler);
    cout << " 野指针问题 ... here" << endl;
    int *p = nullptr;
    // //p = 100;
    *p = 100;

    cout << " 野指针问题 ... here" << endl;
}

// 用一个正常进程，用kill命令手动发信号，看会不会生成核心存储文件
// kill -2  和kill -8分别测试
void test3()
{
    while (true)
    {
        cout << "我是一个正常的进程，正在模拟某种异常:" << getpid() << endl;
        sleep(1);
    }
}

// 测试core dump标志

void test4()
{
    pid_t id = fork();

    if (id == 0)
    {
        sleep(2);
        cout << " 野指针问题 ... here" << endl;
        int *p = nullptr;
        *p = 100;
        cout << " 野指针问题 ... here" << endl;

        exit(0);
    }

    int status = 0;
    waitpid(id, &status, 0);
    // status  15-8 退出码     0-6信号吗   7-code dump标志
    cout << "exit code:" << ((status >> 8) & 0xFF) << endl;     // 获取退出码
    cout << "signal code:" << (status & 0x7F) << endl;          // 获取新号码
    cout << "core dump code:" << ((status >> 7) & 0x1) << endl; // 获取core dump
}

// signal的三个参数
// SIG_ERR    -1     error return
// SIG_DFL    0      default action
// SIG_IGN    1      ingore signal
void test5()
{
    signal(2, SIG_IGN);
    while (1)
    {
        sleep(1);
    }
}

int main()
{
    test5();
    return 0;
}