#include <grpcpp/grpcpp.h>
#include <iostream>
#include <thread>

#include "clientCmdl.h"

int client_main(){
    ClientCommandLine CCLI(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));
    // CCLI.registerNode("localhost", "8080", "Sid's Macbook");
    CCLI.runTerminal();
    return 0;
}
