//管道通信试验
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int pipefile;
    char buf[50];

    if(mkfifo("myfifo", 0666) < 0)
    {
        if(errno == EEXIST)
        {
            printf("the fifo is exist.\n");
        }
        else
        {
            perror("creat myfifo failed!\n");
            exit(1);
        }
    }
    else
    {
        printf("creat success!\n");
    }
    
    pipefile = open("myfifo", O_RDONLY);
    if(pipefile<0)
    {
        printf("Open error");
        exit(1);
    }
    
    while(1)
    {
        if(read(pipefile, buf, 50)<0)
            continue;
        printf("%s is in myfifoi\n", buf);
        memset(buf, 0, 50);
    }
    close(pipefile);
}





