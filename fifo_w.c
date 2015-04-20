#include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <fcntl.h>

    #define N 80

    int main() {
        int out_file;
        int nbyte;
        char buf[N];
        if((mkfifo("myfifo",0666))<0)    //创建有名管道
        {
            if(errno==EEXIST)
            {
                printf("The fifo is exist.\n");
            }
            else{
                perror("creat myfifo failed!\n");
                exit(-1);
            }
        }else{
            printf("created by this process.\n");
        }
        out_file = open("myfifo",O_WRONLY);
        if (out_file < 0) {
            printf("Error opening fifo.");
            exit(1);
        }
        printf("please input something:\n");
        while((nbyte = read(0,buf,N))){
            write(out_file,buf,nbyte);
            printf("please input something:\n");
        }
        close(out_file);
        return 0;
    }

