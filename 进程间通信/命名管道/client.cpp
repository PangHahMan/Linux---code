#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "comm.h"
using namespace std;

int main()
{
    // 1.服务端创建了管道，客户端需不需要创建管道文件呢？
    //  打开对应的文件即可，打开管道文件，返回文件描述符
    int wfd = open(fifoname.c_str(), O_WRONLY);

    if (wfd < 0)
    {
        cerr << errno << ":" << strerror(errno) << endl;
    }

    // 可以进行常规通信了
    char buffer[NUM];
    //回车写入后，服务端读取
    // while (true)
    // {
    //     cout << "请输入你的消息:";
    //     char *msg = fgets(buffer, sizeof(buffer), stdin);
    //     assert(msg);
    //     (void)msg;
    //     buffer[strlen(buffer) - 1] = '\0'; // 此行用于将回车改成\0
    //     //如果输入的是quit表示推出，直接break,server端读取为0,服务端也跟着退出
    //     if (strcasecmp(buffer, "quit") == 0)
    //         break;
    //     // 往管道文件描述符中写入消息，由server端读取
    //     ssize_t n = write(wfd, buffer, strlen(buffer));
    //     assert(n >= 0);
    //     (void)n;
    // }
    //让服务端实时读取代码
    while(true)
    {
        system("stty raw");
        int c = getchar();
        system("stty -raw"); 
        ssize_t n = write(wfd, (char*)&c, sizeof(c));
        
    }
    close(wfd);
    return 0;
}