#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;

__thread int g_val = 100;    //__thread，不想被共享
void *threadRoutine(void *args) {
    string name = static_cast<const char *>(args);

    int cnt = 5;
    while (cnt) {
        cout << name << "g_val:" << g_val++ << ",&g_val" << &g_val << endl;
        sleep(1);
    }
}

int main() {
    pthread_t t1, t2, t3;

    pthread_create(&t1, nullptr, threadRoutine, (void *) "thread 1");
    pthread_create(&t2, nullptr, threadRoutine, (void *) "thread 2");
    pthread_create(&t3, nullptr, threadRoutine, (void *) "thread 3");

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    return 0;
}