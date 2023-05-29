#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

using namespace std;

int tickets = 10000;  // 加锁保证共享资源的安全
pthread_mutex_t mutex;// 后面说

void *threadRoutine(void *name) {
    string tname = static_cast<const char *>(name);

    while (true) {
        pthread_mutex_lock(&mutex);// 所有线程都要遵守这个规则
        if (tickets > 0) {         // tickets == 1; a, b, c,d
            //a,b,c,d
            usleep(2000);// 模拟抢票花费的时间
            cout << tname << " get a ticket: " << tickets-- << endl;
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 后面还有动作
        usleep(1000);//充当抢完一张票，后续动作
    }

    return nullptr;
}

int main() {
    pthread_mutex_init(&mutex, nullptr);

    pthread_t t[4];
    int n = sizeof(t) / sizeof(t[0]);
    for (int i = 0; i < n; i++) {
        char *data = new char[64];
        snprintf(data, 64, "thread-%d", i + 1);
        pthread_create(t + i, nullptr, threadRoutine, data);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(t[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}