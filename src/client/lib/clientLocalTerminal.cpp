#include "clientLocalTerminal.h"
#include "stringOps.h"

template <typename C>
int LocalTerminal::client_connect(C&& args){
    static_assert(has_iterators<C>::value, "Container must have begin and end iterators");
    static_assert(container_holds_type<C, std::string_view>::value 
                || container_holds_type<C, std::string>::value
    , "Container must hold a string_view or string");

    if(args.size() < 3){
        std::cout << client_cmd_usage(client_commands_enum::connect);
        return 0;
    }
    // CCLI.newConnection<std::string, std::string>("localhost", "8080");
    if(CCLI.newConnection(args[1], args[2])){
        std::cout << "Successful connection to server, terminal and all else available\n";
    } else {
        std::cout << "Failed to connect to server, try again\n";
        return 0;
    }
    return 1;

}

template <typename C>
int LocalTerminal::client_register(C&&){
    static_assert(has_iterators<C>::value, "Container must have begin and end iterators");
    static_assert(container_holds_type<C, std::string_view>::value 
                || container_holds_type<C, std::string>::value
    , "Container must hold a string_view or string");

    return 1;
}

int LocalTerminal::new_terminal_command(const std::vector<std::string_view>& cmd){
    client_commands_enum cme = get_arg_number(cmd[0]);
    int result = 0;
    switch(cme){
        case client_commands_enum::connect:
            result = client_connect(cmd);
            break;
        case client_commands_enum::register_node:
            result = client_register(cmd);
            break;
        case client_commands_enum::ping:
            result = client_ping(cmd);
            break;
        case client_commands_enum::start_terminal:
            result = client_start_terminal(cmd);
            break;
        case client_commands_enum::exit:
            result = client_exit(cmd);
            break;
        default:
            break;
    }

    return result;
}

void LocalTerminal::start_terminal(){
    std::string cmd;
    cmd.reserve(100);
    std::vector<std::string_view> args;
    args.reserve(10);
    std::cout << "Starting Terminal\n$:";
    
    while(std::getline(std::cin, cmd)){
        split_string(cmd, args, ' ');

        if(new_terminal_command(args) == -1){
            return;
        };

        std::cout << "\n$:";
    }
}
