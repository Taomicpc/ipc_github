///对共享内存处的数据进行数据排序等处理
//

#include "stdio.h"
#include "stdlib.h"
#include "public.h"
#include "sem_com.c"
//快排法
//
int quickSortOnce(int data[], int low, int high)
{
    int provery = data[low];

    while(1)
    {
        while(high!=low && data[high]>=provery)
        {//等待发现最高位有比对比点小的
            --high;
        }
        if(high==low)
            break;//跳出大循环，结束此次排序

        data[low] = data[high];
        ++low;
        while(high!=low && data[low]<=provery)
        {//等待发现最低位有比对比点大的。
            ++low;
        }
        if(high==low)
            break;//跳出大循环，结束此次排序

        data[high]=data[low];
        --high;    
    }
    data[high]=provery;
    return high;//返回分界点
}

void quickSort(int data[], int low, int high)
{
    int centra;  
        
    centra = quickSortOnce(data, low, high);

    if(centra+1 < high)
    {
        quickSort(data, centra+1, high);
    }
    if(centra-1 > low)
    {
        quickSort(data, low, centra-1);
    }
}

int main(int argv, char* argc[])
{
    int IshmId;
    int IsemId;
    Data_Type* dataPtr=NULL;
    int count;
    
    chartoint(argc[2], &IsemId);
    chartoint(argc[1], &IshmId);

    //printf("\nIn deal shmid = %d", IshmId);

    dataPtr = (Data_Type *)shmat(IshmId, NULL, 0);

    while(1)
    {
        sem_p(IsemId);
        
        if(dataPtr->endflag == true)
            exit(0);

        if(dataPtr->dealfinish == false && dataPtr->dataUsed != 0)
        {
            printf("\nsorting\n");
            quickSort(dataPtr->data, 0, dataPtr->dataUsed-1);
            dataPtr->dealfinish = true;
        }
        
        sem_v(IsemId);
    }
}



