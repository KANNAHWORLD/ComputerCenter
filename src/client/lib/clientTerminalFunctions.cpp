#include <stdarg.h>
#include <iostream>
#include "clientTerminalFunctions.h"
#include "clientCmdl.h"

static ClientCommandLine CCLI;

int client_connect(const std::vector<std::string_view>& args){
    if(args.size() < 3){
        std::cout << client_cmd_usage(client_commands_enum::connect);
        return 0;
    }

    if(CCLI.newConnection(args[1], args[2])){
        std::cout << "Successful connection to server, terminal and all else available" << '\n';
    } else {
        std::cout << "Failed to connect to server, try again with different port" << '\n';
    }
    return 1;
}

int client_register(const std::vector<std::string_view>&){
    return 1;
}

int client_ping(const std::vector<std::string_view>&){
    return 1;
}

int client_exit(const std::vector<std::string_view>&){
    return 1;
}

int client_start_terminal(const std::vector<std::string_view>&){
    return 1;
}
