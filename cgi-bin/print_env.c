#include<stdio.h>
#include<string.h>
#define MAXLINE 8192

int main(int argc,char** argv,char** env){
    char body[MAXLINE];
    memset(body,0,sizeof(body));

    for(int i=0;env[i]!=NULL;i++){
        sprintf(body,"%s%s\n",body,env[i]);
    }
    
    printf("Content-Lenght: %lu\r\n\r\n",strlen(body));
    fflush(stdout);
    printf("%s",body);
    fflush(stdout);
}
