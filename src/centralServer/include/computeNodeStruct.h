/**
 * @file computeNodeStruct.h
 * @brief Defines the ComputeNode struct.
 */

#ifndef COMPUTE_NODE_STRUCT_H
#define COMPUTE_NODE_STRUCT_H

#include <string>

/**
 *
 * 
 * @brief Represents a compute node in the system.
 * 
 * This struct defines the properties and behavior of a compute node in the system.
 * It can be used to store information about a compute node, such as its IP address and ID.
 */
class ComputeNode {
    private:
        std::string ip; ///< The IP address of the compute node.
        std::string port;
        std::string information;
        int id;         ///< The ID of the compute node.

    public:
        
        ComputeNode(const std::string_view& ip = "", const std::string_view& port = "", const std::string_view& info = " ", const int id = -1):
            ip(ip), port(port), information(info), id(id) 
            {};
        
        ComputeNode(const ComputeNode& rhs) noexcept:
            ip(rhs.ip), port(rhs.port), information(rhs.information), id(rhs.id)
            {};
        
        // Move constructor
        ComputeNode(ComputeNode&& rhs) noexcept:
            ip(std::move(rhs.ip)), port(std::move(rhs.port)), information(std::move(rhs.information)), id(rhs.id)
            {};

        ComputeNode& operator=(const ComputeNode& rhs) noexcept{
            this->ip = rhs.ip;
            this->port = rhs.port;
            this->information = rhs.information;
            this->id = rhs.id;
            return *this;
        }

        // Move assignment operator
        ComputeNode& operator=(ComputeNode&& rhs) noexcept{
            this->ip = std::move(rhs.ip);
            this->port = std::move(rhs.port);
            this->information = std::move(rhs.information);
            this->id = rhs.id;
            return *this;
        }

        bool operator==(const ComputeNode& rhs) const noexcept {
            return (this->ip == rhs.ip) && (this->port == rhs.port);
        }

        const std::string& get_ip() const noexcept {
            return ip;
        }

        const std::string& get_port() const noexcept {
            return port;
        }

        const std::string& get_info() const noexcept {
            return information;
        }

        int get_id() const noexcept {
            return id;
        }

        void set_id(int id) noexcept {
            this->id = id;
        }

        void set_info(const std::string& info) noexcept {
            this->information = info;
        }

        void set_ip(const std::string& ip) noexcept {
            this->ip = ip;
        }

        void set_port(const std::string& port) noexcept {
            this->port = port;
        }
};

/**
 * @brief Hash functor to hash Compute Nodes
*/
struct ComputeNodeHash{
    std::hash<std::string> string_hash_func;
    std::size_t operator()(const ComputeNode& n) const {
        return string_hash_func(n.get_ip()) ^ string_hash_func(n.get_port());
    }
};


#endif // COMPUTE_NODE_STRUCT_H
