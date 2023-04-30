#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

//展示老的位图
void showBlock(sigset_t *oset)
{
    // 1-31的信号
    for (int i = 1; i <= 31; i++)
    {
        if (sigismember(oset, i))
        {
            cout << "1";
        }
        else
        {
            cout << "0";
        }
    }
    cout << endl;
}

// 对二号信号做屏蔽
void test1()
{
    // 这里只是在用户层面上进行设置
    sigset_t set, oset;
    // 清空两个信号
    sigemptyset(&set);
    sigemptyset(&oset);

    sigaddset(&set, SIGINT); // SIGINT 添加SIGINT信号到set中

    // 设置进入进程，谁调用，设置谁
    // 1. 2号信号没有反应， 2.老的信号屏蔽字block位图为全0
    sigprocmask(SIG_SETMASK, &set, &oset);

    int cnt = 0;
    while (true)
    {
        showBlock(&oset);  //展示老的位图
        sleep(1);
        cnt++;

        if (cnt == 10)
        {
            cout << "recover block" << endl;
            sigprocmask(SIG_SETMASK, &oset, &set);  //设置老的信号屏蔽字，也就是恢复
            showBlock(&set);   //下节课
        }
    }
}

int main()
{
    test1();
    return 0;
}