#include<csapp.h>
#include<stdbool.h>
extern char** environ;
//filename="./cgi-bin/test.exe"
//cgi_args="a=x&b=y&c=z"
//cgi_args="a&b&c"
void service_dynamic(int fd,char* filename,char* cgi_args){
    char headers[MAXLINE];
    sprintf(headers,"HTTP/1.0 200 OK\r\n");
    sprintf(headers,"%sServer: HJ Tiny Web\r\n",headers);
    sprintf(headers,"%sContent-Type: text/plain; charset=UTF-8\r\n",headers);
    Rio_writen(fd,headers,strlen(headers));

    if(Fork()==0){
        setenv("QUERY_STRING",cgi_args,true);
        Dup2(fd,STDOUT_FILENO);
        if(execle(filename,filename,NULL,environ)==-1){
            printf("execle error! errno:%d-%s",errno,strerror(errno)); 
        }
    }
    Wait(NULL);
}
