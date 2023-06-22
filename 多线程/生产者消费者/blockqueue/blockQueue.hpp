#pragma once

#include <iostream>
#include <pthread.h>
#include <queue>

const int gcap = 5;

//不要认为，阻塞队列只能放整数字符串之类的，也可以放对象
template<class T>
class BlockQueue {

public:
    BlockQueue(const int cap = gcap) : _cap(cap) {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&consumer_cond, nullptr);
        pthread_cond_init(&productor_cond, nullptr);
    }

    bool isfull() { return q.size() == _cap; }
    bool isEmpty() { return q.empty(); }
    void push(const T &in) {
        pthread_mutex_lock(&_mutex);//细节1： 一定要保证，在任何时候，都是符合条件，才进行生产
        while (isfull()) {          //1.我们只能在临界区内部，判断临界资源是否就绪！注定了我们在当前一定是持有锁的

            //2.要让线程进行休眠等待，不能持有锁等待！
            //3.就注定了，pthread_cond_wait要有锁的释放的能力，需要将锁传到参数
            // 我休眠(切换)了，我醒来的时候，在哪里往后执行呢？
            pthread_cond_wait(&productor_cond, &_mutex);
            //4.当线程醒来的时候，注定了继续从临界区内部继续运行！ 因为我是在临界区被切走的
            //5.注定了当我们的线程被唤醒的时候，继续在pthread_cond_wait函数处向后运行,又要重新申请锁，申请成功才会彻底返回
        }
        //没有满的，就要让他进行生产
        q.push(in);
        pthread_cond_signal(&consumer_cond);//唤醒消费者
        pthread_mutex_unlock(&_mutex);
    }


    void pop(T &out) {
        pthread_mutex_lock(&_mutex);

        while (isEmpty()) {
            pthread_cond_wait(&consumer_cond, &_mutex);
        }

        out = q.front();
        q.pop();

        pthread_cond_signal(&productor_cond);//唤醒生产者
        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue() {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&consumer_cond);
        pthread_cond_destroy(&productor_cond);
    }

private:
    std::queue<T> q;
    int _cap;
    pthread_mutex_t _mutex;       //互斥锁
    pthread_cond_t consumer_cond; //消费者对应的条件变量，空就wait
    pthread_cond_t productor_cond;//生产者对应的条件变量，满就wait
};