#include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <string.h>
    #define N 80

    int main(void) {
        int in_file;
        int count = 1;
        char buf[N];
        if((mkfifo("myfifo",0666))<0)//创建有名管道
        {
            if(errno==EEXIST)//管道已经存在
            {
                printf("The fifo is exist.\n");
            }
            else{
                printf("creat myfifo failed!\n");
                exit(-1);
            }
        }
        else
        {
            printf("created by this process.\n");    
        }
        in_file = open("myfifo",O_RDONLY);
        if (in_file < 0) {
            printf("Error in opening.\n");
            exit(1);
        }

        while ((count = read(in_file,buf,N)) > 0)
        {
            printf("received from fifo: %s\n", buf);
            memset(buf,0,N);
        }
        close(in_file);
        return 0;
    }

