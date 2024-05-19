#include <iostream>
#include <thread>
#include <string>

#include <unistd.h>
#include <filesystem>
#include "sys_command.h"

void function(){
    std::cout << "Hello from thread" << std::endl;
}

extern void start_central_server();
extern int client_main();

int main(int argc, char** argv) {

    std::string command = "ls";
    run_system_command(command);
    run_system_command(command);
    run_system_command(command);
    // std::string p = std::filesystem::current_path().string() + "/output.txt"; 
    // std::cout << p << std::endl;
    // execl("/bin/zsh", "/bin/zsh", "-c", ("ls > " + p).c_str(), NULL);

    // if(argc == 1) start_central_server();
    // else client_main();
    // function();
}
