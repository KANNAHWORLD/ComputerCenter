#include <string>
#include <vector>
#include <fstream>
#include "spinlock.h"
#include <thread>
#include <grpcpp/grpcpp.h>
#include "command.grpc.pb.h"
#include "command.pb.h"
#include <filesystem>
#include <atomic>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// static Spinlock outputFileLock; 
// Setting the quit flag to true makes the stop.
void handle_terminal_output(::grpc::ServerReaderWriter< ::CLOutput, ::CLInput>* stream, int pipe_id, std::atomic<bool>* quit_flag){
    ::CLOutput client_out;
    std::string bash_output;
    bash_output.resize(1000);
    int bytes_read = 0;
    std::cout << "Handling terminal output on server side\n";
    do{
        bytes_read = read(pipe_id, bash_output.data(), bash_output.size());
        if(bytes_read > 0){
            client_out.set_output(bash_output);
            stream->Write(client_out);
            std::cout << "Bytes read from bash output: " << bytes_read << "\n";
            std::cout << client_out.output() << "\n";
        }
    } while(bytes_read != 0 && !quit_flag->load(std::memory_order_relaxed));
}

void run_system_terminal(::grpc::ServerReaderWriter< ::CLOutput, ::CLInput>* stream){
    // Untested for windows
    std::atomic<bool> quit_flag(false);

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
        int serverToBash[2];
        int bashToServer[2];
        pipe(serverToBash);
        pipe(bashToServer);

        int pid = fork();
        if(pid==0){
            // Do not need to write from server to bash
            // Do not need to read in bash to server
            close(serverToBash[1]);
            close(bashToServer[0]);

            if(bashToServer[1] == -1 || serverToBash[0] == -1){
                std::cerr << "Error with pipelines\n";
                goto terminate;
            }

            // Standard input/output for the bash process is the pipeline between the process
            // Allows dynamic input/output bash process
            if (dup2(serverToBash[0], STDIN_FILENO) == -1 || dup2(bashToServer[1], STDOUT_FILENO) == -1) {
                std::cerr << "Error chaing stdin or stdout";
                goto terminate;
            }
            execl("/bin/zsh", "/bin/zsh", NULL);
            // Process never reaches here unless goto is used
        terminate:
            exit(0);
        }
        // Do not need write end in bash to server
        // Do not need read end in server to bash
        std::cout << "new ZSH Session with PID: " << pid << "\n";
        close(serverToBash[0]);
        close(bashToServer[1]);

        ::CLInput client_in;
        std::thread terminal_output(handle_terminal_output, stream, bashToServer[0], &quit_flag);

        while(stream->Read(&client_in)){
            std::cout << "Server: Running Command " << client_in.input().c_str() << ' ' << client_in.input().size() << "\n";
            write(serverToBash[1], client_in.input().c_str(), client_in.input().size());
            write(serverToBash[1], "\n", 1);
        }

        close(serverToBash[1]);
        close(bashToServer[0]);
        waitpid(pid, NULL, 0);
        
        // Kill the output managing thread eventually
        quit_flag.store(true, std::memory_order_relaxed);

        terminal_output.join();
    #endif

    std::cout << "Server: Terminal Ended\n";
}
