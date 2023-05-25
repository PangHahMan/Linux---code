#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define NUM 10
using namespace std;

class ThreadData
{
public:
    ThreadData(const string &name, int id, const time_t creatTime)
        : _name(name), _id(id), _createTime(creatTime)
    {
    }

    ~ThreadData()
    {
    }

public:
    string _name;
    int _id;
    long _createTime;
};

void *thread_run(void *args) // args是pthread_create 第四个参数
{
    // char *name = (char *)args;
    ThreadData *td = static_cast<ThreadData *>(args);
    while (true)
    {
        cout << "thread is running ,name:" << td->_name << " creatTime:" << td->_createTime << " id:" << td->_id << endl;
        sleep(1);
        pthread_exit((void *)1);
    }
    delete td;

    // return nullptr;
}

int main()
{
    pthread_t tids[NUM]; // t是输出型参数，为线程id
    for (int i = 0; i < NUM; i++)
    {
        char tname[64];

        snprintf(tname, 64, "thread-%d", i + 1);
        ThreadData *td = new ThreadData(tname, i + 1, time(nullptr));

        pthread_create(tids + i, nullptr, thread_run, td);
        sleep(1);
    }

    void *ret = nullptr;
    for (int i = 0; i < NUM; i++)
    {
        int n = pthread_join(tids[i], &ret); // 第二个参数是二级指针
        if (n != 0)
            cerr << "pthread_join erro" << endl;

        cout << "thread quit::" << (long)ret << endl;
    }

    cout << "all thread quit..." << endl;

    return 0;
}