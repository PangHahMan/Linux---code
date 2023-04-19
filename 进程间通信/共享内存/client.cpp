#include "comm.h"
int main()
{
    // 1.创建key
    key_t k = GetKey();
    cout << "server:" << toHex(k) << endl;
    // 2.获取共享内存
    int shmid = GetShm(k, gsize);
    cout << "client shmid:" << shmid << endl;

    // 3. 将自己和共享内存关联起来
    char *start = AttachShm(shmid);

    // 4. 去关联
    DetachShm(start);
    return 0;
}