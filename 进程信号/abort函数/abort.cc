#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
using namespace std;

void myhandler(int signo)
{
    cout << "get a signl:" << signo << endl;
}

int main(int argc, char *argv[])
{
    signal(SIGABRT, myhandler);

    while (true)
    {
        cout << "begin" << endl;
        sleep(1);
        abort(); // 给自己发送指定信号
        // abort函数不会循环，为什么？  这是C语言的接口，被封装过，abort后会自动退出程序
        cout << "end" << endl;
    }

    return 0;
}
