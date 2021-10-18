#include<stdbool.h>
#include<csapp.h>

#include"tinyweb.h"

//支持GET请求
//控制台打印请求行和请求头
//支持
//  静态内容(./[filename] || index.html):html,gif,png,jpg text
//  动态内容(./cgi-bin/cginame[?var=x&let=y]):
void doit(int fd){
    char buf[MAXLINE];
    char method[10];
    char http_version[10];
    char uri[MAXLINE];
    char filename[MAXLINE];
    char cgi_args[MAXLINE];

    memset(buf,0,sizeof(buf));
    memset(method,0,sizeof(method));
    memset(http_version,0,sizeof(http_version));
    memset(uri,0,sizeof(uri));
    memset(filename,0,sizeof(filename));
    memset(cgi_args,0,sizeof(cgi_args));
    

    rio_t rio;
    Rio_readinitb(&rio,fd);

    Rio_readlineb(&rio,buf,sizeof(buf));
    sscanf(buf,"%s %s %s",method,uri,http_version);
    
    if(strcmp(method,"GET")!=0){
         client_error(fd,405,"Only the 'GET' method is supported."); 
         return;
    }

    print_request(&rio);

    bool is_static=parse_uri(uri,filename,cgi_args);  //也在这里填充缺省文件
    struct stat file_stat;
    if(stat(filename,&file_stat)==-1){
        client_error(fd,404,"File Not found.");
        return;
    } 

    if(is_static==true){  //静态内容
        if(!S_ISREG(file_stat.st_mode) || !(S_IRUSR & file_stat.st_mode)){
            client_error(fd,403,"Tiny web couldn't read the file.");
            return;
        }

        service_static(fd,filename,file_stat.st_size);

    }else{  //动态内容
        if(!S_ISREG(file_stat.st_mode) || !(S_IXUSR & file_stat.st_mode)){
            client_error(fd,403,"Tiny web couldn't run the CGI program.");
            return;
        }
        
        service_dynamic(fd,filename,cgi_args);
    }

}
