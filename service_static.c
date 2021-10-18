#include<csapp.h>

void get_filetype(char* filename,char* file_type){
//只支持 html,gif,png,jpg 其余全部判定为text/plain 
    char* ext=NULL;
    
    ext=strrchr(filename,'.');

    if(strcmp(ext,".html")==0){
        strcpy(file_type,"text/html");

    }else if(strcmp(ext,".gif")==0){
        strcpy(file_type,"image/gif");

    }else if(strcmp(ext,".png")==0){
        strcpy(file_type,"image/png");

    }else if(strcmp(ext,".jpg")==0){
        strcpy(file_type,"image/jpeg");

    }else if(strcmp(ext,".ico")==0){
        strcpy(file_type,"image/icon");

    }else{
        strcpy(file_type,"text/plain");

    }
}


void service_static(int fd,char* filename,__off_t filesize){
    char headers[MAXLINE];
    char file_type[20];

    memset(headers,0,sizeof(headers));
    memset(file_type,0,sizeof(file_type));

    get_filetype(filename,file_type);

    sprintf(headers,"HTTP/1.0 200 OK\r\n");
    sprintf(headers,"%sServer: HJ Tiny Web\r\n",headers);
    sprintf(headers,"%sContent-Type: %s; charset=UTF-8\r\n",headers,file_type);  
    sprintf(headers,"%sContent-Length: %ld\r\n\r\n",headers,filesize);
    //Content-Length 只算请求体的长度.
    
    int request_fd=Open(filename,O_RDONLY,0);
    char* request_map=Mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,request_fd,0);
    Close(request_fd);
    Rio_writen(fd,headers,strlen(headers));
    Rio_writen(fd,request_map,filesize);
    Munmap(request_map,filesize);
    printf("response headers:\n%s",headers);
    fflush(stdout);
}
