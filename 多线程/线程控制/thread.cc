#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define NUM 10
using namespace std;

enum
{
    OK = 0,
    ERROR
};

class ThreadData
{
public:
    ThreadData(const string &name, int id, const time_t creatTime, int top)
        : _name(name), _id(id), _createTime(creatTime), _status(OK), _result(0), _top(top)
    {
    }

    ~ThreadData()
    {
    }

public:
    // 输入的
    string _name;
    int _id;
    long _createTime;

    // 返回的
    int _status; // 求状态
    int _top;    // 求和
    int _result; // 结果
};

void *thread_run(void *args) // args是pthread_create 第四个参数
{
    // char *name = (char *)args;
    ThreadData *td = static_cast<ThreadData *>(args);

    // 计算求和
    for (int i = 1; i <= td->_top; i++)
    {
        td->_result += i;
    }

    cout << td->_name << "cal done!" << endl;
    pthread_exit(td);
    // while (true)
    // {
    //     cout << "thread is running ,name:" << td->_name << " creatTime:" << td->_createTime << " id:" << td->_id << endl;
    //     sleep(1);
    //     pthread_exit((void *)1);
    // }
    // delete td;

    // return nullptr;
}

int main()
{
    pthread_t tids[NUM]; // t是输出型参数，为线程id
    for (int i = 0; i < NUM; i++)
    {
        char tname[64];

        snprintf(tname, 64, "thread-%d", i + 1);
        ThreadData *td = new ThreadData(tname, i + 1, time(nullptr), 100 + 5 * i);

        pthread_create(tids + i, nullptr, thread_run, td);
        sleep(1);
    }

    void *ret = nullptr;
    for (int i = 0; i < NUM; i++)
    {
        // pthread_join 用于等待一个线程结束并收集其返回值的函数。
        int n = pthread_join(tids[i], &ret); // 第二个参数是二级指针
        if (n != 0)
            cerr << "pthread_join erro" << endl;

        ThreadData *td = static_cast<ThreadData *>(ret); // 将ret转换为ThreadData*指针
        if (td->_status == OK)
        {
            cout << td->_name << " 计算的结果是: " << td->_result << " (它要计算的是[1, " << td->_top << "])" << endl;
        }

        delete td;
        // cout << "thread quit::" << (long)ret << endl;
    }

    cout << "all thread quit..." << endl;

    return 0;
}