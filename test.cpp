#include<unistd.h>
#include<iostream>
#include<thread>
#include<cerrno>
#include<regex>
using std::string;

int main(){
    string str="abc";
    auto pos=str.rfind('?');
    string str2 = str.substr(pos+1);
    std::cout<<pos<<std::endl;
    std::cout<<str2<<std::endl;
}
