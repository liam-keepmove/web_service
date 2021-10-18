#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAXLINE 8192

int main(int argc,char** argv,char** env){
    char body[MAXLINE];
    memset(body,0,sizeof(body));

    char* param=getenv("QUERY_STRING");
    if(param==NULL){
        fprintf(stderr,"QUERY_STRING error\n");
        return 1;
    }
            
    char* p=strchr(param,'&');

    if(p!=NULL){
        int one=atoi(param);
        int two=atoi(p+1);
        sprintf(body,"%d",one+two);
    }
    

    printf("Content-Lenght: %lu\r\n\r\n",strlen(body));
    printf("%s",body);
    fflush(stdout);
}
