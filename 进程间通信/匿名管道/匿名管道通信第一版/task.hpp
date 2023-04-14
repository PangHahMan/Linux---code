#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <unistd.h>
using namespace std;
typedef void (*func_t)(); // 函数指针

void PrintLog()
{
    cout << "pid:" << getpid() << "打印日志任务，正在被执行 " << endl;
}

void InsertMySQL()
{
    cout << "pid:" << getpid() << "执行数据库任务,正在被执行" << endl;
}

void NetRequst()
{
    cout << "pid:" << getpid() << "执行网络请求任务,正在被执行..." << endl;
}

// 约定,每一个command都必须是4字节
#define COMMAND_LOG 0
#define COMMADN_MYSQL 1
#define COMMAND_REQUEST 2

struct Task
{
    Task()
    {
        func.push_back(PrintLog);
        func.push_back(InsertMySQL);
        func.push_back(NetRequst);
    }
    // command的值为0，1，2也就是func的下标值
    void Execute(int command)
    {
        if (command >= 0 && command < func.size())
        {
            func[command](); // 合法就调用函数
        }
    }
    vector<func_t> func;

    ~Task(){};
};