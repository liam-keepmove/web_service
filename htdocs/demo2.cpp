#include<iostream>
using std::string;

int main(){
    string one="abc";
    string two("d\0fa",4);
    string three=one+two;
    std::cout<<three<<std::endl;
    std::cout<<three.size()<<std::endl;
    std::cout<<two.size()<<std::endl;

}
