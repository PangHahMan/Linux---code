#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
using namespace std;

// 自定义handler函数
// static void handler(int signo)
// {
//     cout << "对特定信号:" << signo << "执行捕捉动作" << endl;
// }

// static的作用是使函数的作用域仅限于声明它的源文件中。这意味着该函数不能从其他源文件访问。这也称为“内部链接”。
static void PrintPending(const sigset_t &pending)
{
    cout << "当前进程的pending位图:";
    for (int signo = 1; signo <= 31; signo++)
    {
        // sigismember函数用于判断一个信号是否在一个信号集合中。
        if (sigismember(&pending, signo))
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
}

static void handler(int signo)
{
    cout << "对特定信号:" << signo << "执行捕捉动作" << endl;

    int cnt = 30;
    while (cnt)
    {
        cnt--;

        sigset_t pending;
        sigemptyset(&pending);
        sigpending(&pending);
        PrintPending(pending);
        sleep(1);
    }
}

void test1()
{
    int cnt = 0;
    // 1.屏蔽2号信号
    sigset_t set, oset; // 恢复老的信号屏蔽字，oset
    // 1.1初始化(清0)
    sigemptyset(&set);
    sigemptyset(&oset);

    // 1.2将2号信号添加到set中
    sigaddset(&set, SIGINT);
    // 1.3 将新的信号屏蔽字设置到进程中
    sigprocmask(SIG_BLOCK, &set, &oset);

    // 2.0 设置对2号信号的自定义捕捉
    signal(2, handler);
    // 2.while获取进程的pending信号集，并01打印
    while (true)
    {
        // 2.1先获取pending信号集
        sigset_t pending;
        sigemptyset(&pending); // 不是必须的
        int n = sigpending(&pending);
        assert(n == 0);
        (void)n; // 保证不会出现编译时的warning

        // 2.2 打印，方便我们查看
        PrintPending(pending);

        // 2.3 休眠一下
        sleep(1);

        // 2.4 10s之后，恢复对所有信号的bolck动作
        if (cnt++ == 10)
        {
            cout << "解除对2号信号的屏蔽" << endl;
            sigprocmask(SIG_SETMASK, &oset, nullptr);
        }
    }
}

// sigaction函数
void test2()
{
    struct sigaction act, oldact;
    memset(&act, 0, sizeof(act));
    memset(&oldact, 0, sizeof(oldact));
    act.sa_handler = handler;
    act.sa_flags = 0;

    sigemptyset(&act.sa_mask);
    // 同时屏蔽3 4 5
    sigaddset(&act.sa_mask, 3);
    sigaddset(&act.sa_mask, 4);
    sigaddset(&act.sa_mask, 5);
    // 对2号信号捕捉
    sigaction(2, &act, &oldact);

    while (true)
    {
        cout << getgid() << endl;
        sleep(1);
    }
}

int main()
{
    test2();
    return 0;
}