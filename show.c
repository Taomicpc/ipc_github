///对共享内存处的数据进行显示
//

#include "stdio.h"
#include "stdlib.h"
#include "public.h"
#include "sem_com.c"

int main(int argv, char* argc[])
{
    int IshmId;
    int IsemId;
    Data_Type* dataPtr=NULL;
    int count;

    chartoint(argc[2], &IsemId);
    chartoint(argc[1], &IshmId);

    //printf("\nIn show shmid = %d", IshmId);

    dataPtr = (Data_Type *)shmat(IshmId, NULL, 0);

    while(1)
    {
        sem_p(IsemId);
        
        if(dataPtr->endflag == true)
            exit(0);

        if(dataPtr->dealfinish == true)
        {
            printf("\nAfter sort data is:\n");

            for(count=0; count < dataPtr->dataUsed;)
            {
                printf("%d ", dataPtr->data[count]);
                if((++count)%10 == 0)
                    printf("\n");
            }
            
            printf("\n--------all---------\n\n");
            dataPtr->dataUsed = 0;//显示后清空共享内存
            dataPtr->dealfinish = false;
        }

        sem_v(IsemId);
    }
}



