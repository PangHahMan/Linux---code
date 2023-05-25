#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <ctime>

using namespace std;

void *threadRun(void *args)
{
    const char *name = static_cast<const char *>(args);

    int cnt = 5;
    while (cnt)
    {
        cout << name << " is running: " << cnt-- << " obtain self id: " << pthread_self() << endl;
        sleep(1);
    }

    pthread_exit((void *)11);

    // PTHREAD_CANCELED; // 被线程取消的返回值#define PTHREAD_CANCELED ((void *) -1)   
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRun, (void *)"thread 1");
    // sleep(3);

    // pthread_cancel(tid);   

    void *ret = nullptr;
    pthread_join(tid, &ret);
    cout << " new thread exit : " << (int64_t)ret << "quit thread: " << tid << endl; //(int64_t)ret 因为ret是void*  Linux下环境是64位，void* 8字节
    return 0;
}