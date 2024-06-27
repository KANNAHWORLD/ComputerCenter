#include <grpcpp/grpcpp.h>
#include <iostream>
#include <thread>
#include "clientLocalTerminal.h"

int client_main(){
    LocalTerminal CCLI;
    // CCLI.registerNode("localhost", "8080", "Sid's Macbook");
    std::string connection = "connect";
    std::string ip = "localhost";
    std::string port = "8080";
    CCLI.new_terminal_command({connection, ip, port});
    // CCLI.new_terminal_command({"connect", "localhost", "8080"});
    return 0;
}
