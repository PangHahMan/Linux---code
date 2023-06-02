#include "Thread.hpp"
#include "LockGuard.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;


int tickets = 1000;                                // 加锁保证共享资源的安全
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//全局锁

//多线程抢票，可重入函数，需要设置互斥锁
void threadRoutine(void *args) {
    std::string message = static_cast<const char *>(args);
    while (true) {
        {
            LockGuard lockguard(&mutex);
            if (tickets > 0) {// tickets == 1; a, b, c,d
                //a,b,c,d
                usleep(2000);// 模拟抢票花费的时间
                cout << message << " get a ticket: " << tickets-- << endl;
            } else {
                break;
            }
        }
        usleep(2000);//充当抢完一张票，后续动作
    }
}


// void threadRun(void *args) {
//     std::string message = static_cast<const char *>(args);
//     int cnt = 10;
//     while (cnt) {
//         cout << "我是一个线程，" << message << ",cnt:" << cnt-- << endl;
//         sleep(1);
//     }
// }

int main() {
    Thread t1(1, threadRoutine, (void *) "hello1");
    Thread t2(2, threadRoutine, (void *) "hello2");
    Thread t3(3, threadRoutine, (void *) "hello3");
    Thread t4(4, threadRoutine, (void *) "hello4");

    t1.run();
    t2.run();
    t3.run();
    t4.run();

    t1.join();
    t2.join();
    t3.join();
    t4.join();


    return 0;
}