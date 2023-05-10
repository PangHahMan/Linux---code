#include <cstdio>
#include <signal.h>

volatile int quit = 0;   //保证内存可见性

void handler(int signal)
{
    quit = 1;
    printf("change quit from 0 to 1\n");
    printf("quit:%d", quit);
}

int main()
{
    signal(2, handler);

    while (!quit)
        ; // 注意这里我们故意没有携带while的代码块，故意让编译器认为在main中，quit只能被检测
    printf("正常退出\n");
    return 0;
}