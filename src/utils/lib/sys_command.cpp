#include <string>
#include <vector>
#include <fstream>
#include "spinlock.h"
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

static Spinlock outputFileLock; 
static int outputFileId = 0;

std::string run_system_command(std::string& command){

    std::scoped_lock lock(outputFileLock);
    int commandLength = command.size();
    
    std::string outputFile = std::filesystem::current_path().string();
    outputFile += "/.";
    outputFile += std::to_string(outputFileId);
    outputFile += "out.txt";
    
    outputFileId += 1;

    #ifdef _WIN32
        std::string command("powershell.exe ");
        command += command;
        command +=  " > ";
        command += outputFile;

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Create the child process
        if (!CreateProcess(
            NULL,   // No module name (use command line)
            const_cast<char*>(command.c_str()),        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory
            &si,            // Pointer to STARTUPINFO structure
            &pi)            // Pointer to PROCESS_INFORMATION structure
            )
        {
            // Handle error...
            std::cout << "Error forking\n";
        } else {

            WaitForSingleObject(pi.hProcess, INFINITE);
            // Close process and thread handles
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    #else
        std::vector<char *> argv {"/bin/zsh", "-c"};
        
        command += " > ";
        command += outputFile;
        argv.push_back(const_cast<char*>(command.c_str()));
        argv.push_back(NULL);
        
        int fpid = fork();
        if(fpid == 0){
            execv("/bin/zsh", argv.data());
        }
        
        waitpid(fpid, NULL, 0);
    #endif

    std::ifstream file(outputFile, std::ifstream::in);
    std::string output = "";
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            output += line;
            output += '\n';
        }
    }
    file.close();
    // printf("Output file: %s\n", output.c_str());
    return std::move(output);
}
