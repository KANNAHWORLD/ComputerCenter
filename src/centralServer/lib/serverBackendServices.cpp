#include <unordered_set>
#include <fstream>
#include <string_view>
#include <iostream>
#include "spinlock.h"
#include "computeNodeStruct.h"
#include "serverBackendServices.h"


// Stores all compute nodes that are available in the system
static std::unordered_set<ComputeNode, ComputeNodeHash> nodes;
static Spinlock node_lock;

static const std::string NODES_FILE = "nodes.txt";

/**
 * @todo: Determine the file format for the nodes file
 * @todo: Maybe this could be switched to a database
 * Not thread safe
 */
void load_nodes(){
    std::ifstream file(NODES_FILE, std::ifstream::in);
    
    std::string line;
    std::string_view ip;
    std::string_view port;
    std::string_view info;
    int firstSpace = 0;
    int secondSpace = 0;
    std::cout << "Loading nodes\n";
    if(file.is_open()){
        line.reserve(100);
        int id = -1;

        while(std::getline(file, line)){
            firstSpace = line.find(' ');
            secondSpace = line.find(' ', firstSpace + 1);
            ip = std::string_view(line.data(), firstSpace);
            port = std::string_view(line.data() + firstSpace + 1, secondSpace - firstSpace - 1);
            info = std::string_view(line.data() + secondSpace + 1, line.size() - secondSpace - 1);
            std::cout << ip << ' ' << port << ' ' << info << '\n';
            id += 1;
            nodes.emplace(ip, port, info, id);
        }
    }
    file.close();
    std::cout << "All nodes are loaded\n";
}

/**
 * @todo Determine the file format for the nodes file
 * Saves the available nodes to the file system.
 * Not thread safe
 */
void save_nodes(){
    std::ofstream file(NODES_FILE, std::ofstream::out);
    std::cout << "Saving nodes\n";
    if(file.is_open()){
        std::scoped_lock lock(node_lock);
        for(auto& node : nodes){
            file << node.get_ip() << ' ' << node.get_port() << ' ' << node.get_info() << '\n';
        }
    }
    file.close();
    std::cout << "All nodes are saved\n";
}

/**
 * @brief Adds a new compute node to the server
 * Thread safe
*/
bool add_new_compute_node(const std::string& ip, const std::string& port, const std::string& info) noexcept{
    ComputeNode newNode(ip, port, info);
    std::scoped_lock lock(node_lock);
    nodes.emplace(std::move(newNode));
    return true;
}

/**
 * @brief Retrieves all compute nodes in the system
*/
std::string string_retrieve_all_nodes(){
    std::string all_nodes;
    all_nodes.reserve(1000);
    std::scoped_lock lock(node_lock);
    for(auto& node : nodes){
        all_nodes += node.get_ip();
        all_nodes += ' '; 
        all_nodes += node.get_port();
        all_nodes += ' '; 
        all_nodes += node.get_info();
        all_nodes += '\n';
    }
    return all_nodes;
}

const std::unordered_set<ComputeNode, ComputeNodeHash>& retrieve_all_nodes(){
    return nodes;
}
