#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#define MAX 1024
#define SEP " "
int split(char *commandstr, char *argv[])
{
    // argv[i]以NULL结束
    assert(commandstr);
    assert(argv);

    argv[0] = strtok(commandstr, SEP);
    if (argv[0] == NULL)
        return -1;
    int i = 1;
    while ((argv[i++] = strtok(NULL, SEP))) // 代码优化，argv的结尾是NULL
        ;
    return 0;
}

void showEnv()
{
    extern char **environ;
    int i = 0;
    for (i = 0; environ[i]; i++)
    {
        printf("%d:%s\n", i, environ[i]);
    }
}

int main()
{
    int last_exit = 0;
    char myenv[32][256];
    int env_index = 0;
    extern char **environ;
    while (1)
    {
        char commandstr[MAX] = {0};
        char *argv[64] = {NULL};
        printf("[ikun@mymachine currpath]# "); // 打印提示信息
        fflush(stdout);                        // 刷新标准输出流缓冲区，以便在输出前将所有缓冲的输出数据发送到终端
        // 从标准输入流中读取一行字符串，存储到字符数组 commandstr 中
        char *s = fgets(commandstr, sizeof(commandstr), stdin);
        assert(s); // 断言，判断指针 s 不为 NULL，否则程序会终止运行
        (void)s;   // 保证在 release 方式发布的时候，因为去掉 assert 了，
                   // 所以 s 就没有被使用，而带来的编译告警，什么都没做，但是充当了一次使用
        // 例如输入字符串 "abcd\n\0"，在读取后会在末尾自动加上 '\n'，因此需要将其替换为 '\0'，以便后续使用
        commandstr[strlen(commandstr) - 1] = '\0';
        // ls -a -l ->"la" "-a" "l"  拆成3各字符串   strtok

        int n = split(commandstr, argv);
        if (n != 0)
            continue;
        
        //cd 目录名   目录名就是argv[1]
        if (strcmp(argv[0], "cd") == 0)
        {
            if (argv[1] != NULL)
                chdir(argv[1]);   //int chdir(const char *path); 其中，path参数是要切换到的目标目录的路径名。如果切换成功，chdir()函数返回0；否则返回-1，
            continue;
        }
        else if (strcmp(argv[0], "export") == 0)
        {
            if (argv[1] != NULL)
            {
                strcpy(myenv[env_index], argv[1]);  //将argv[1]的参数复制到myenv中，然后借助putenv上传环境变量，注意要将index++
                putenv(myenv[env_index++]);
            }
            continue;
        }
        else if (strcmp(argv[0], "env") == 0)
        {
            showEnv();  //打印env即可，借助C语言environ 环境变量表
            continue;
        }
        else if (strcmp(argv[0], "echo") == 0)
        {
            // echo $PATH  echo ? 是获取最近的退出码
            const char *target_env = NULL;
            if (argv[1][0] == '$')
            {
                //模拟$?  不是? 就是获取环境变量
                if (argv[1][1] == '?')
                {
                    printf("%d\n", last_exit);  //last_exit 就是父进程获取的退出码
                    continue;
                }
                else
                {
                    target_env = getenv(argv[1] + 1); // 获取$后面的path环境变量
                }

                if (target_env != NULL)
                {
                    printf("%s=%s\n", argv[1] + 1, target_env);
                }         
            }
            continue;
        }
        
        // 添加颜色  --color=auto
        if (strcmp(argv[0], "ls") == 0)
        {
            // 在最后的位置加上--color=auto设置颜色
            int pos = 0;
            //找最后的位置
            while (argv[pos])
                pos++;
            argv[pos] = "--color=auto";
        }
        //  让子进程去做
        pid_t id = fork();
        assert(id >= 0);
        (void)id;

        if (id == 0)
        {
            // child
            execvp(argv[0], argv); // argv[0] 就是ls  execvp函数会在系统环境变量中找可执行程序，
            // argv其中第一个字符串表示可执行文件的名称，后面的字符串表示传递给可执行文件的命令行参数。
            exit(0);
        }
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        if (ret > 0)
        {
            last_exit = WEXITSTATUS(status);
        }
        // printf("%s\n", commandstr);
    }

    return 0;
}
