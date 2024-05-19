#include <grpcpp/grpcpp.h>
#include <iostream>
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

    private:
        std::unique_ptr<::CommandLine::Stub> _stub;

};

int client_main(){
    ClientCommandLine CCLI(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));
    CCLI.registerNode("localhost", "8080", "Sid's Macbook");
    return 0;
}
