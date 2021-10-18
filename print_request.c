#include<csapp.h>

//打印请求头
void print_request(rio_t* rio){
    char buf[MAXLINE];
    Rio_readlineb(rio,buf,sizeof(buf));
    while(strcmp(buf,"\r\n")!=0){
        Rio_readlineb(rio,buf,sizeof(buf));
        printf("%s",buf);
        fflush(stdout);
    }
    printf("\r\n");
    fflush(stdout);
}

