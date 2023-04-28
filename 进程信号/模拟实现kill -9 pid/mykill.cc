#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
using namespace std;

void Usage(string proc)
{
    cout << "Usage: \n\t ";
    cout << proc << " 信号编号 目标进程" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    int signo = atoi(argv[1]);
    int target_id = atoi(argv[2]);

    int n = kill(target_id, signo);
    if (n != 0)
    {
        cerr << errno << ":" << strerror(errno) << endl;
    }
    return 0;
}

