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

    //one=1&two=2&flag=zh
    char* num1=strchr(param,'=');
    char* num2=strchr(num1+1,'=');
    char* flag=strchr(num2+1,'=');
    char* p1=strchr(param,'&');
    char* p2=strchr(p1+1,'&');

    if(num1!=NULL && num2!=NULL && flag!=NULL){
        *p1='\0';
        *p2='\0';
        int one=atoi(num1+1);
        int two=atoi(num2+1);
        sprintf(body,"%d",one*two);
        if(*(flag+1)=='z'){
            sprintf(body,"Hello zhang jia jun!");
        }else{
            sprintf(body,"Hello Huang Jin!");
        }
    }
    

    printf("Content-Lenght: %lu\r\n\r\n",strlen(body));
    printf("%s",body);
    fflush(stdout);
}
