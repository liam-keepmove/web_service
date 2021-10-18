#pragma once

#ifndef TINYWEB_H
#define TINYWEB_H

#include<csapp.h>
void doit(int);
void print_request(rio_t*);
int parse_uri(char*,char*,char*); //uri,filename,cgi_args
void client_error(int,int,char*); //fd,status_code,error_msg
void service_static(int,char*,__off_t);  //fd,filename,file_size mmap use file_size
void service_dynamic(int,char*,char*);  //fd,filename,cgi_args

#endif
