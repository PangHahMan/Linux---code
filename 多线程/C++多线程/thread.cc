#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void run1() {
    while (true) {
        cout << "thread 1" << endl;
        sleep(1);
    }
}

void run2() {
    while (true) {
        cout << "thread 2" << endl;
        sleep(1);
    }
}

void run3() {
    while (true) {
        cout << "thread 3" << endl;
        sleep(1);
    }
}
int main() {
    thread th1(run1);
    thread th2(run2);
    thread th3(run3);

    th1.join();
    th2.join();
    th3.join();
    return 0;
}