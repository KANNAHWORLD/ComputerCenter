#include <grpcpp/grpcpp.h>
#include <iostream>
#include <thread>
#include "command.grpc.pb.h"

class ClientCommandLine {
    private:

        /**
         * Stores the stub for accessing the gRPC server
         */
        std::unique_ptr<::CommandLine::Stub> _stub;

    public:
        ClientCommandLine(std::shared_ptr<grpc::Channel> channel);

        /**
         * Ping the server to make sure it is still active
         */
        ::grpc::Status ping();

        /**
         * registers 
         */
        void registerNode(std::string&& ip, std::string&& port, std::string&& info);

        /**
         * Runs the terminal for sending commands to the server and receiving outputs.
         * 
         * This function creates a gRPC client context and a client reader-writer stream
         * to communicate with the server. It reads commands from the standard input and
         * sends them to the server using the stream. The function also sets a quit flag
         * to signal the output monitor thread to stop. After sending all the commands,
         * it waits for 2 seconds and then signals the end of writes to the stream.
         */
        void runTerminal();

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
                                    std::atomic<bool>* quit_flag) noexcept;
};

