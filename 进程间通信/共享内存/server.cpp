#include "comm.h"
#include <unistd.h>
int main()
{
    // 1.创建key
    key_t k = GetKey();
    cout << "server:" << toHex(k) << endl;

    // 2.创建共享内存
    int shmid = Creatshm(k, gsize);
    cout << "server shmid:" << shmid << endl;

    //3.将自己和共享内存关联起来
    char* start = AttachShm(shmid);

    //通信代码在这里

    //4. 去关联
    DetachShm(start);
    
    // 3.删除共享内存
    DelShm(shmid);
    return 0;
}