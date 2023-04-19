#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#define pathname "."
#define proj_id 0x6666

const int gsize = 4096;

key_t GetKey()
{
    key_t k = ftok(pathname, proj_id);
    if (k == -1)
    {
        cerr << errno << ":" << strerror(errno) << endl;
        exit(1);
    }

    return k;
}

string toHex(int x)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "0x%x", x);
    return buffer;
}

// static变量 表示全局由server和client共享
static int CreatShmHelper(key_t k, int size, int flag)
{
    int shmid = shmget(k, gsize, flag);
    if (shmid == -1)
    {
        cerr << errno << ":" << strerror(errno) << endl;
        exit(2);
    }
    return shmid;
}

int Creatshm(key_t k, int size)
{
    umask(0);
    return CreatShmHelper(k, size, IPC_CREAT | IPC_EXCL | 0666); // 创建要保证是最新的所以带上IPC_CREAT | IPC_EXCL
}

int GetShm(key_t k, int size)
{
    return CreatShmHelper(k, size, IPC_CREAT);
}

char *AttachShm(int shmid)
{
    char *start = (char *)shmat(shmid, nullptr, 0);
    return start;
}

void DetachShm(char *start)
{
    int n = shmdt(start);
}

void DelShm(int shmid)
{
    int n = shmctl(shmid, IPC_RMID, nullptr);
}