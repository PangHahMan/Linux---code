#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;
void *threadRoutine(void *args) {
    string name = static_cast<const char *>(args);
    int cnt = 5;
    while (cnt) {
        cout << name << ":" << cnt-- << endl;
        sleep(1);
    }
    return nullptr;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *) "thread 1");//(void *) "thread 1表示threadRoutine的参数args
    //pthread_detach(tid);
    // sleep(1);
    // pthread_detach(tid);
    // int n = pthread_join(tid, nullptr);
    // if (n != 0) {
    //     cerr << "error:" << n << ":" << strerror(n) << endl;
    // }

    sleep(5);//主线程睡眠,主线程退了,整个进程就退出了
    //cout << "主线程sleep了5结束" << endl;
    return 0;
}