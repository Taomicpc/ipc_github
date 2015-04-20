//对共享内存处的数据进行改写
//

#include "stdio.h"
#include "stdlib.h"
#include "public.h"
#include "sem_com.c"

const int constData[NUM]={34,23,432,43,4,664,5};

int main(int argv, char* argc[])
{
    int IshmId;
    int IsemId;
    Data_Type* dataPtr=NULL;
    int count;
    bool Bfinishinput = false;
    key_t ipcKey;

    if(argv >= 3)
    {
        chartoint(argc[2], &IsemId);
        chartoint(argc[1], &IshmId);
    }
    else if(argv == 2)
    {
        chartoint(argc[1], &IshmId);
    }
    else
    {
        ipcKey = ftok("./shm", 'a');
        if(ipcKey == -1)
            perror("ftok error");
    
        IshmId = shmget(ipcKey, sizeof(Data_Type),S_IRUSR | S_IWUSR | IPC_CREAT | 0777);//0666是ubuntu下此操作需要的校验
    
        if(IshmId == -1)
        {
            perror("shmget error");
            return;
        }
    }

    //printf("\nIn input shmid = %d", IshmId);

    dataPtr = (Data_Type *)shmat(IshmId, NULL, 0);
    //printf("dataptr = 0x%x\n",(int)dataPtr);
    //printf("dataptr->size = %d\n", dataPtr->size);

#if 0
    memset(dataPtr, 0, sizeof(Data_Type));
    dataPtr->size = NUM;
#endif

    while(1)
    {
        sem_p(IsemId);//P操作
        
        if(dataPtr->endflag == true)
            exit(0);

        if(dataPtr->dataUsed == 0)
        {
            printf("Buffer is emtry!you can input num(<%d)\n", dataPtr->size);
            printf("Use 'e' to enter\n");

            char charbuf;
            for(count=0, Bfinishinput=false;
                    count < dataPtr->size && Bfinishinput == false; 
                        ++count)
            {
                //dataPtr->data[count] = constData[count];
                //scanf("%d ", dataPtr->data + count);
                 while(scanf("%d", &(dataPtr->data[count])) != 1)
                 {
                     if((charbuf = getchar())=='e')
                     {
                         Bfinishinput = true;
                         break;
                     }
                     else if(charbuf == 'q')
                     {
                         Bfinishinput = true;
                         dataPtr->endflag = true;
                         break;
                     }
                 }
            }

            printf("\nWell!you input %d number\n", count-1);
            dataPtr->dataUsed = count-1;//更新已使用的空间
            dataPtr->dealfinish = false;
        }
        //sleep(1);
        
        sem_v(IsemId);
    }
}






