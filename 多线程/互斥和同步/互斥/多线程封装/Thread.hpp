#pragma once
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <string>


class Thread {
public:
    typedef void (*func_t)(void *);
    typedef enum {
        NEW = 0,
        RUNNING,   //运行
        EXITED     //退出
    } ThreadStatus;//进程状态

public:
    Thread(int num, func_t func, void *args)
        : _tid(0), _status(NEW), _func(func), _args(args) {
        char name[127];
        snprintf(name, sizeof(name), "thread-&d", num);
        _name = name;
    }

    int status() {
        return _status;
    }

    std::string threadnmae() {
        return _name;
    }

    pthread_t threadid() {
        if (_status == RUNNING) {
            return _tid;
        } else {
            //std::cout << "thread is not running,no tid" << std::endl;
            return 0;
        }
    }

    // 这是类的成员函数，而类的成员函数，具有默认参数this
    // 但是会有新的问题：static成员函数，无法直接访问类属性和其他成员函数
    static void *runHelper(void *args) {
        Thread *ts = (Thread *) args;
        (*ts)();
        return nullptr;
    }

    void operator()() {//仿函数
        if (_func != nullptr) _func(_args);
    }

    void run() {
        int n = pthread_create(&_tid, nullptr, runHelper, this);
        if (n != 0) {
            exit(1);
        }
        _status = RUNNING;
    }

    void join() {
        int n = pthread_join(_tid, nullptr);
        if (n != 0) {
            std::cerr << "main thread join thread" << _name << "error" << std::endl;
            return;
        }
        _status = EXITED;
    }

    ~Thread() {
    }

private:
    pthread_t _tid;
    std::string _name;
    func_t _func;//线程未来要执行的回调
    void *_args;
    ThreadStatus _status;
};