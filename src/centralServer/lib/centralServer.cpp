/**
 * @file centralServer.cpp
 * @author Siddharth Bansal (bansalsi@usc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-05-04
 * 
 * @copyright Copyright (c) 2024
 * 
 * @file centralServer.cpp
 * @brief Implementation of the central server functionality.
 * 
 * This file contains the implementation of the central server, which is responsible for
 * reading and storing compute nodes and their IP addresses. Usually all connections from client nodes
 * are first routed through the central server, which then forwards the request to the appropriate compute node.
 */
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
#include "computeNodeStruct.h"
#include "command.grpc.pb.h"
#include "spinlock.h"
#include "serverTerminal.h"
#include "serverBackendServices.h"
#include "sys_command.h"
#include <grpcpp/server_builder.h>
#include <unordered_set>
#include <functional>
#include "centralServer.h"


class CommandLineImp final : public CommandLine::Service {
    public:
        virtual ::grpc::Status registerNode(::grpc::ServerContext* context, const ::NodeDetails* request, ::Empty* response) override {
            std::cout << "New Compute Node at: " << request->ip() << "\n";
            add_new_compute_node(request->ip(), request->port(), request->information());
            return ::grpc::Status::OK;
        }

        virtual ::grpc::Status ping(::grpc::ServerContext* context, const ::Empty* request, ::Empty* response) override {
            std::cout << "Server: Ping.\n";
            return ::grpc::Status::OK;
        }

        virtual ::grpc::Status getNodes(::grpc::ServerContext* context, const ::Empty* request, ::CLOutput* response) override {
            std::cout << "Server: Getting Nodes.\n";
            response->set_output(retrieve_all_nodes());
            return ::grpc::Status::OK;
        }

        virtual ::grpc::Status runTerminal(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::CLOutput, ::CLInput>* stream) override {
            std::cout << "Server: Starting and Running Terminal\n";
            run_system_terminal(stream);
            return ::grpc::Status::OK;
        }
};

// Builds the server and begins running it
static inline void build_server(const std::string& ip_port){
    // as of now, don't do anything
    grpc::ServerBuilder builder;
    builder.AddListeningPort(ip_port, grpc::InsecureServerCredentials());

    CommandLineImp my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
    return;
}

/**
 * @brief Starts the central server.
 * Essentially the main 
 * 
 * This function starts the central server, performs necessary operations to
 * initialize the server.
 */
void start_central_server(const std::string& ip_port){
    
    std::cout << "Central server started." << std::endl;
    load_nodes();
    // Build the server and run it
    std::thread server_thread(build_server, ip_port);
    
    // Read in a variable to 
    // Start other actions here
    manageTerminal();

    save_nodes();
    // DO not unlock before terminating since it is not needed
    return;
}
