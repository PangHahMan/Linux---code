#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

using namespace std;

const int num = 5;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   //初始化条件变量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//初始化互斥锁
void *active(void *args) {
    string name = static_cast<const char *>(args);

    while (true) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);//调用的时候，会自动释放锁
        cout << name << "活动" << endl;
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t tids[num];

    for (int i = 0; i < num; i++) {
        char *name = new char[32];

        snprintf(name, 32, "thread-%d", i + 1);
        pthread_create(tids + i, nullptr, active, name);
    }

    sleep(3);

    while (true) {
        cout << "main thread wakeup thread.." << endl;
        //pthread_cond_signal(&cond);//唤醒线程
        pthread_cond_broadcast(&cond);  //同时唤醒所有线程
        sleep(1);
    }

    for (int i = 0; i < num; i++) {
        pthread_join(tids[i], nullptr);
    }
    return 0;
}