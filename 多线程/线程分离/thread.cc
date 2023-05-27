#include <iostream>
#include <string>
#include <pthread.h>
#include <cstring>
using namespace std;
void *threadRoutine(void* args)
{
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *)"thread 1");
    return 0;
}