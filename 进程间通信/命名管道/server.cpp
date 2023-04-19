#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "comm.h"
using namespace std;
int main()
{
    // 1.创建管道文件
    umask(0); // 这个设置并不影响系统的默认配置，只会影响当前进程
    int n = mkfifo(fifoname.c_str(), mode);
    if (n != 0)
    {
        cout << errno << ":" << strerror(errno) << endl;
        return 1;
    }
    cout << "create fifo file success" << endl;
    // 2.让服务端直接开启管道文件
    int rfd = open(fifoname.c_str(), O_RDONLY);
    if (rfd <= 0)
    {
        cout << errno << ":" << strerror(errno) << endl;
        return 2;
    }
    cout << "open fifo success,begin ipc" << endl;

    // 3.正常通信
    char buffer[NUM];
    while (true)
    {
        buffer[0] = '\0';
        ssize_t n = read(rfd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = '\0';
            //cout << "client:" << buffer << endl;
            printf("%c",buffer[0]);
            fflush(stdout);
        }
        //如果
        else if (n == 0)
        {
            cout << "client quit,me too" << endl;
            break;
        }
        else
        {
            cout << errno << ":" << strerror(errno) << endl;
            break;
        }
    }

    // 关闭不要的fd
    close(rfd);
    //关闭管道文件
    unlink(fifoname.c_str());
    return 0;
}