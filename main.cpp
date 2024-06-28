#include <iostream>
#include <thread>
#include <string>

#include <unistd.h>
#include <filesystem>
#include "sys_command.h"
#include "centralServer.h"
#include <fcntl.h>

void function(){
    std::cout << "Hello from thread" << std::endl;
}

extern int client_main();

int main(int argc, char** argv) {


    // std::string p = std::filesystem::current_path().string() + "/output.txt"; 
    // std::cout << p << std::endl;
    // execl("/bin/zsh", "/bin/zsh", "-c", ("ls > " + p).c_str(), NULL);

    if(argc == 1) start_central_server();
    else client_main();
    function();
}
