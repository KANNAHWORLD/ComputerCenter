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

        /**
         * Stores if the connection is active
         */
        bool _connectionActive;

        /**
         * Stores the connection IP address and port
         */
        std::string _connectionIpPort;

        /**
         * Changes state of ClientCommandLine to active or inactive after each stub call
         * Maintains the connection state
         * @param status The status of the last gRPC call
         * @returns void
         */
        __header_always_inline void updateConnectionState(const ::grpc::Status& status){
            this->_connectionActive = status.ok();
        }

    public:
        ClientCommandLine(std::shared_ptr<grpc::Channel> channel);
        ClientCommandLine();

        /**
         * Establishes a new connection with the server.
         * Creates a new _stub object with the given server IP and port.
         * Pings the server to make sure it is active
         * @param ip The IP address of the server
         * @param port The port number of the server
         * @return True if the connection is successful, false otherwise
         */
        // bool newConnection(std::string&&, std::string&&, bool);
        bool newConnection(std::string&&, const std::string&&);

        __header_always_inline bool newConnection(std::string& ip, std::string& port){
            return this->newConnection(std::string(ip), std::string(port));
        }

        __header_always_inline bool newConnection(const std::string_view& ip, const std::string_view& port){
            return this->newConnection(std::string(ip), std::string(port));
        }

        /**
         * Ping the server to make sure it is still active
         */
        const ::grpc::Status ping();

        /**
         * @returns if the current connection is active (last ping worked)
         * 
         */
        __header_always_inline bool connectionActive() const noexcept {
            return this->_connectionActive;
        }

        /**
         * registers nodes into the server details. This serves the purpose
         * of knowing which nodes are connected to the server and can be accessed
         * @param must be of type std::string or std::string_view
         */

        template <typename T>
        void registerNode(const T&&, const T&&, const T&&);
        

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

