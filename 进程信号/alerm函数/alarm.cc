#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

int count = 0;

void handler(int signo)
{
    cout << "get a signo:" << signo << "count:" << count << endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, handler);
    alarm(1);
    while (true)
        count++;
    return 0;
}
