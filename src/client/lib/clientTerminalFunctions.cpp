#include <stdarg.h>
#include <iostream>
#include "clientTerminalFunctions.h"
#include "clientCmdl.h"

static const ClientCommandLine CCLI;

int client_connect(const std::vector<std::string_view>& args){
    if(args.size() < 3){
        std::cout << client_cmd_usage(client_commands_enum::connect);
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
