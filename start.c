#include<csapp.h>
#include<stdbool.h>
#include"tinyweb.h"

/* 迭代服务器
 * 迭代的,阻塞式的,处理每个连接请求.
 * csapp 672page
 * 2021年10月11日17:45:16
 * */


//打开并返回指定服务(端口)的监听套接字,成功返回非负文件描述符,失败返回-1
int open_listenfd_my(char* service){
    struct addrinfo hints={.ai_family=AF_INET,
                           .ai_socktype=SOCK_STREAM,
                           .ai_flags=AI_ADDRCONFIG|AI_PASSIVE
    };
    struct addrinfo* result=NULL;
    int listenfd=-1;
    int ret_code=0;
    if((ret_code=getaddrinfo(NULL,service,&hints,&result))!=0){
        fprintf(stderr,"errno:%d %s\n",ret_code,gai_strerror(ret_code));
        return ret_code;
    }else{
        for(struct addrinfo* p=result;p!=NULL;p=p->ai_next){
            if((listenfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
                listenfd=-1;
                continue;
            }
            if(bind(listenfd,p->ai_addr,p->ai_addrlen)==-1){
                close(listenfd);
                listenfd=-1;
                continue;
            }
            if(listen(listenfd,1024)==-1){
                close(listenfd);
                listenfd=-1;
                continue;
            }
            break;
        }
        freeaddrinfo(result);
        return listenfd;
    }
}


//监听连接,并调用处理函数
int main(int argc,char** argv){
    if(argc!=2) {
        fprintf(stderr,"usage:%s <port>\n",argv[0]);
        return 1;
    }
    int listenfd=-1;
    if((listenfd=open_listenfd_my(argv[1]))==-1){
        fprintf(stderr,"get listenfd error.\n");
        return 1;
    }
    struct sockaddr clientaddr={};
    socklen_t clientlen=0;
    while(true){
        int clientfd=accept(listenfd,&clientaddr,&clientlen);
        if(clientfd==-1){
            fprintf(stderr,"accpet function error.\n");
            return 1;
        }
        doit(clientfd);
        close(clientfd);
    }
}
