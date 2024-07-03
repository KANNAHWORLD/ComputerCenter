/**
 * @brief Runs the system terminal and handles input/output communication with the client.
 * 
 * This function is responsible for running the system terminal and managing the input/output communication
 * with the client. It uses gRPC to receive input commands from the client and sends the output of the
 * terminal back to the client. The function supports both Windows and Unix-like systems.
 * 
 * @param stream A pointer to the gRPC ServerReaderWriter object for sending/receiving messages.
 */
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
#include "stdio.h"
#include <csignal>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <unistd.h>
#endif


/**
 * Handles the terminal output on the server side.
 * 
 * This function reads the output from a pipe and sends it to the client using gRPC.
 * It continuously reads from the pipe until either the pipe is closed or the quit flag is set.
 * 
 * @param stream A pointer to the gRPC ServerReaderWriter object for sending output to the client.
 * @param pipe_id The file descriptor of the pipe from which to read the output.
 * @param quit_flag A pointer to an atomic boolean flag indicating whether the operation should be quit.
 */
void handle_terminal_output(::grpc::ServerReaderWriter< ::CLOutput, ::CLInput>* stream, 
                            int pipe_id,
                            std::atomic<bool>* quit_flag){
    
    ::CLOutput client_out;
    std::string bash_output;

    #define read_buffer_size 1024

    bash_output.resize(read_buffer_size);
    
    // set the pipe reading to be non-blocking
    fcntl(pipe_id, F_SETFL, O_NONBLOCK);

    int bytes_read = 0;
    // std::cout << "Handling terminal output on server side\n";

    do{

        bytes_read = read(pipe_id, bash_output.data(), read_buffer_size);
        if(bytes_read > 0){
            client_out.set_output(std::string(bash_output.begin(), bash_output.begin() + bytes_read));
            stream->Write(client_out);
            bytes_read = 0;
        } else if(bytes_read == -1) {
            usleep(200);
        } else {
            break;
        }

    } while(!quit_flag->load(std::memory_order_relaxed));
}


static constexpr char 
terminal_new_command_prompt_command[] = "echo cleint @ remote $:";

static constexpr char
terminal_background_wait[] = "\n wait \n";

/**
 * @brief Executes a system terminal command.
 * 
 * This function is responsible for executing a system terminal command. It takes a gRPC server reader-writer 
 * stream as input, which is used to communicate with the client. The function supports both Windows and Unix-like systems.
 * 
 * On Windows, the function uses the `CreateProcess` function to create a child process and execute the command using 
 * PowerShell. The output of the command is redirected to a file specified by `outputFile`.
 * 
 * On Unix-like systems, the function uses pipes to establish communication between the server and the terminal. 
 * It forks a child process and executes the command using `/bin/zsh`. The input and output of the command are redirected through the pipes.
 * 
 * @param stream A pointer to the gRPC server reader-writer stream.
 */
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
            close(serverToBash[0]);
            close(bashToServer[1]);
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

        // Set the priority of the forked process to be high
        setpriority(PRIO_PROCESS, pid, -20);

        ::CLInput client_in;

        // Start the output managing thread, Writes output to the stream for visibility on the client side
        std::thread terminal_output(handle_terminal_output, stream, bashToServer[0], &quit_flag);

        while(stream->Read(&client_in)){
            // std::cout << "Server: Running Command " << client_in.input().c_str() << ' ' << client_in.input().size() << "\n";
            write(serverToBash[1], client_in.input().c_str(), client_in.input().size());
            write(serverToBash[1], terminal_background_wait, sizeof(terminal_background_wait) - 1);
            write(serverToBash[1], terminal_new_command_prompt_command, sizeof(terminal_new_command_prompt_command) - 1);
            write(serverToBash[1], terminal_background_wait, sizeof(terminal_background_wait) - 1);
        }

        close(serverToBash[1]);
        close(bashToServer[0]);
        waitpid(pid, NULL, 0);
        
        // Kill the output managing thread eventually
        quit_flag.store(true, std::memory_order_relaxed);
        // terminal_output.join();
        kill(pid, SIGKILL);

    #endif

    std::cout << "Server: Terminal Ended\n";
}
