#include "clientLocalTerminal.h"
#include "stringOps.h"

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
};

