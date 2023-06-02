#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

using namespace std;

int tickets = 10000;// 加锁保证共享资源的安全
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //全局锁

struct TData {
    TData(const string &name, pthread_mutex_t *mutex)
        : _name(name), _pmutex(mutex) {}

    ~TData() {
        if (_pmutex != nullptr) {
            pthread_mutex_destroy(_pmutex);
            delete _pmutex;
        }
    }

    string _name;
    pthread_mutex_t *_pmutex = nullptr;
};



//多线程抢票，可重入函数，需要设置互斥锁
void *threadRoutine(void *args) {
    TData *td = static_cast<TData*>(args);

    while (true) {
        pthread_mutex_lock(td->_pmutex);// 所有线程都要遵守这个规则
        if (tickets > 0) {         // tickets == 1; a, b, c,d
            //a,b,c,d
            usleep(2000);// 模拟抢票花费的时间
            cout << td->_name << " get a ticket: " << tickets-- << endl;
            pthread_mutex_unlock(td->_pmutex);//解锁
        } else {
            pthread_mutex_unlock(td->_pmutex);//解锁
            break;
        }

        // 后面还有动作
        usleep(100);//充当抢完一张票，后续动作
    }

    return nullptr;
}


int main() {
    pthread_mutex_t mutex;              //局部锁
    pthread_mutex_init(&mutex, nullptr);//初始化

    pthread_t t[4];
    int n = sizeof(t) / sizeof(t[0]);
    for (int i = 0; i < n; i++) {
        char *name = new char[64];
        snprintf(name, 64, "thread-%d", i + 1);
        TData* td = new TData(name,&mutex);
        
        pthread_create(t + i, nullptr, threadRoutine, td);//创建线程
    }

    for (int i = 0; i < n; i++) {
        pthread_join(t[i], nullptr);//等待
    }

    pthread_mutex_destroy(&mutex);//销毁锁
    return 0;
}