#include "blockQueue.hpp"
#include "task.hpp"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

void *consumer(void *args) {
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);

    while (true) {
        sleep(1);
        Task t;
        //1.将数据从blockqueue中获取 -- 获取到了数据

        bq->pop(t);
        t();//拿到了x,y,op,需要调用operator()进行计算
        //2.结合某种业务逻辑，处理数据！
        cout << "consumer Task:" << t.formatArg() << t.formatRes() << endl;
    }
}

void *productor(void *args) {
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    std::string opers = "+-*/%";
    while (true) {
        sleep(1);
        //1.先通过某种渠道获取数据
        //int data = rand() % 10 + 1;
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = opers[rand() % opers.size()];
        Task t(x, y, op);

        //2.将数据推送到blockqueue -- 完成生产过程
        bq->push(t);
        cout << "productor Task:" << t.formatArg() << "?" << endl;
    }
}

int main() {
    srand((uint64_t) time(nullptr) ^ getpid());//获得一个随机数
    //BlockQueue<int> *bq = new BlockQueue<int>();
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    //单生产和单消费
    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&p, nullptr, productor, bq);


    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    delete bq;
    return 0;
}