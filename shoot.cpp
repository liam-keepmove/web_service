//
// Created by ajin on 7/31/2022.
//

#include"HttpServer.hpp"

int main(int argc, char** argv) {
    //运行web服务
    if (argc != 2) {
        fprintf(stderr,"<usage>:%s port\n",argv[0]);
        return 1;
    }
    HttpServer httpServer{atoi(argv[1]), 7};
    httpServer.run();
    return 0;
}