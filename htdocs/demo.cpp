#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>

int main(){
    struct stat fileStat={};
    stat("./favicon.ico", &fileStat);
    printf("file size:%lu\n",fileStat.st_size);
    int fd=open("./favicon.ico",O_RDONLY,0);
    char* fdbuf=nullptr;
    fdbuf=(char*)malloc(fileStat.st_size);
    printf("%p\n",fdbuf);
    int n=read(fd,fdbuf,fileStat.st_size);
    printf("read function return code:%d\n",n);
    printf("%s\n",fdbuf);
    write(1,fdbuf,fileStat.st_size);

}
