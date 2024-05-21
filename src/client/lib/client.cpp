#include <grpcpp/grpcpp.h>
#include <iostream>
#include <thread>
#include "command.grpc.pb.h"

class ClientCommandLine {
    public:
        ClientCommandLine(std::shared_ptr<grpc::Channel> channel)
            : _stub(CommandLine::NewStub(channel)) {}
        
        void registerNode(std::string ip, std::string port, std::string info){
            ::NodeDetails newNodeDetails;
            newNodeDetails.set_ip(ip);
            newNodeDetails.set_port(port);
            newNodeDetails.set_information(info);

            grpc::ClientContext CC;
            ::Empty reply;

            grpc::Status RPC_stat = _stub->registerNode(&CC, newNodeDetails, &reply);
            std::cout << RPC_stat.error_code() << '\n';
        }

        /**
         * Runs the terminal for sending commands to the server and receiving outputs.
         * 
         * This function creates a gRPC client context and a client reader-writer stream
         * to communicate with the server. It reads commands from the standard input and
         * sends them to the server using the stream. The function also sets a quit flag
         * to signal the output monitor thread to stop. After sending all the commands,
         * it waits for 2 seconds and then signals the end of writes to the stream.
         */
        void runTerminal(){

            ::CLInput input;
            ::CLOutput output;
            grpc::ClientContext CC;
            std::string command_input;
            std::atomic<bool> quit_flag(false);
            std::shared_ptr<grpc::ClientReaderWriter< ::CLInput, ::CLOutput>> stream(_stub->runTerminal(&CC));

            std::thread serverOutputManager(terminalOutputMonitor, stream, &quit_flag);

            while(std::getline(std::cin, command_input)){
                input.set_input(command_input);
                stream->Write(input);
            }
            
            // Set quit flag to quit the output monitor thread
            quit_flag.store(true, std::memory_order_relaxed);
            sleep(2);
            stream->WritesDone();
            serverOutputManager.join();
            std::cout << "Terminal session ended.\n";
        }

    private:

        /**
         * Monitors the terminal output from the server.
         * 
         * This function reads the output from the server through the given gRPC stream
         * and prints it to the console. It continues reading until either the stream
         * ends or the quit flag is set to true.
         * 
         * @param stream A shared pointer to the gRPC ClientReaderWriter object for
         *               reading server output.
         * @param quit_flag A pointer to an atomic boolean flag indicating whether the
         *                  monitoring should be stopped.
         */
        static void terminalOutputMonitor(std::shared_ptr<grpc::ClientReaderWriter< ::CLInput, ::CLOutput>> stream, 
                                    std::atomic<bool>* quit_flag) noexcept {

            ::CLOutput output;
            while(stream->Read(&output) && !(quit_flag->load(std::memory_order_relaxed))){
                std::cout << output.output();
            }
        }

        std::unique_ptr<::CommandLine::Stub> _stub;

};
#define rstr kjlsdkjf
int client_main(){
    ClientCommandLine CCLI(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));
    // CCLI.registerNode("localhost", "8080", "Sid's Macbook");
    CCLI.runTerminal();
    return 0;
}
