#include <iostream>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main()
{
    while (true)
    {
        cout << "我是进程,我的PID是:" << getpid() << endl;
        sleep(1);
    }
    return 0;
}