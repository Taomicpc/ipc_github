//用于测试进程通信的文件，此处用共享内存的方式
//此为主程序，由他去创建多个进程，并创建共享内存
//

//fetch test

#include "stdio.h"
#include "stdlib.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include "public.h"
#include "memory.h"
#include <linux/fs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

//使用自定义函数的信号量操作
//#include "sem_com.c"
//使用库函数的信号量操作
#include <semaphore.h>

int main(int argc, char* argv[])
{

//--------------共享内存创建--------------------
    key_t ipcKey;
    int IshmId;
    Data_Type* dataPtr=NULL;

    ipcKey = ftok("./shm", 'a');
    if(ipcKey == -1)
        perror("ftok error");

    IshmId = shmget(ipcKey, sizeof(Data_Type),S_IRUSR | S_IWUSR | IPC_CREAT | 0777);//0666是ubuntu下此操作需要的校验

    if(IshmId == -1)
    {
        perror("shmget error");
        return;
    }
    //else
    //    printf("\nIn main shmid = %d", IshmId);

    dataPtr = (Data_Type *)shmat(IshmId, NULL, 0);
   
    if(dataPtr == (void *)-1)
    {
        perror("main.c shmat error");
        return;
    }

//    printf("dataptr = 0x%x\n",(int)dataPtr);
    
    memset(dataPtr, 0, sizeof(Data_Type));
    dataPtr->size = NUM;//记录共享内存的大小
    dataPtr->dealfinish = false;
    dataPtr->dataUsed = 0;
    dataPtr->endflag = false;

//-------创建共享内存信号量-----------------

    int IsemId;
    key_t sem_key;

    sem_key=ftok("./sem",'a');
    //以0666且create mode创建一个信号量，返回给sem_id
    IsemId=semget(sem_key,1,0666|IPC_CREAT);
    //将sem_id设为1
    init_sem(IsemId,1);

//-------------创建进程-------------------
    pid_t pid = -1;
    char SshmId[20];//用于接收字串型的shmId
    char SsemId[20];//用于接收字串型的shmId

    sprintf(SshmId, "%d", IshmId);
    sprintf(SsemId, "%d", IsemId);

    char* execvInput[] = {"./input", SshmId, SsemId, NULL};
    char* execvShow[] = {"./show", SshmId, SsemId, NULL};
    char* execvDeal[] = {"./deal", SshmId, SsemId, NULL};

    if(fork()==0)//写入子进程
    {
        execv("./input", execvInput, NULL);//启动共享资源写入进程
        perror("execv input error");
    }

    if(fork()==0)//显示子进程
    {
        execv("./show", execvShow, NULL);//启动共享资源打印
        perror("execv show error");
    }

    if(fork()==0)//数据处理子进程
    {
        execv("./deal", execvDeal, NULL);//启动共享资源写入进程
        perror("execv deal error");
    }

    int status;

    while(wait(&status) != -1);//一直等待其产生的子进程都结束,每一次等待都是休眠

    printf("Main exit");
    shmctl(IshmId, IPC_RMID, 0);

}





