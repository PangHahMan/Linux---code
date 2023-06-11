#pragma once

#include <iostream>
#include <pthread.h>
#include <queue>

template<class T>
class BlockQueue {

public:
    BlockQueue() {
    }

    void push(const T &in) {
    }

    void pop(const T &out) {
    }

    ~BlockQueue() {
    }

private:
    std::queue<T> q;
    int _cap;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};