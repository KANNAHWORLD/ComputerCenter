#include <grpcpp/grpcpp.h>
#include <iostream>
#include <thread>
#include "command.grpc.pb.h"
#include "typeCheck.h"

class RemoteTerminal {
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
        RemoteTerminal(std::shared_ptr<grpc::Channel> channel);
        RemoteTerminal();

        /**
         * Establishes a new connection with the server.
         * Creates a new _stub object with the given server IP and port.
         * Pings the server to make sure it is active
         * @param ip The IP address of the server
         * @param port The port number of the server
         * @return True if the connection is successful, false otherwise
         */
        // bool newConnection(std::string&&, std::string&&, bool);
        template <typename T1, typename T2>
        bool newConnection(T1&& ip, T2&& port){

            static_assert(string_like<T1>::value, "\nIP must be a string or string_view\n");
            static_assert(string_like<T2>::value, "\nPort must be a string or string_view\n");

            std::string tip(ip);
            auto Channel = grpc::CreateChannel(tip.append(":").append(port), grpc::InsecureChannelCredentials());
            this->_stub = CommandLine::NewStub(Channel);
            if(::grpc::Status::OK.ok() == this->ping().ok()){
                this->updateConnectionState(::grpc::Status::OK);
                this->_connectionIpPort = std::move(ip);
                return true;
            }
            return false;
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
        void registerNode(const T&& ip, const T&& port, const T&& info){
            
            static_assert(string_like<T>::value, "IP, Port and Info must be of type string or string_view");

            ::NodeDetails newNodeDetails;
            newNodeDetails.set_ip(ip);
            newNodeDetails.set_port(port);
            newNodeDetails.set_information(info);

            grpc::ClientContext CC;
            ::Empty reply;

            grpc::Status RPC_stat = _stub->registerNode(&CC, newNodeDetails, &reply);
            this->updateConnectionState(RPC_stat);

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

