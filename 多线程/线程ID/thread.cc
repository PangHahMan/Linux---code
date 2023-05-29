#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;


string hexAddr(pthread_t tid) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "0x%x", tid);

    return buffer;
}

void *threadRoutine(void *args) {
    string name = static_cast<const char *>(args);
    int cnt = 5;
    while (cnt) {
        cout << name << ":" << cnt-- << ":" << hexAddr(pthread_self()) << endl;
        sleep(1);
    }
    return nullptr;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *) "thread 1");//(void *) "thread 1表示threadRoutine的参数args

    while (true) {
        cout << "main thread_id:" << hexAddr(pthread_self()) << " new thread_id:" << hexAddr(tid) << endl;
        sleep(1);
    }

    return 0;
}