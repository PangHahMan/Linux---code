#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
using namespace std;

// rasie函数 自己给自己发信号
void myhandler(int signo)
{
    cout << "get a signl:" << signo << endl;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, myhandler);

    // 循环收到信号
    while (true)
    {
        sleep(1);
        raise(SIGINT);
    }
    return 0;
}
