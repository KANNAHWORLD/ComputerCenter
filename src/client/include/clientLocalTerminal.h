#include <unordered_map>
#include "clientRemoteTerminal.h"
#include "typeCheck.h"
#include <iostream>

const std::string client_help[] = {
    {"Connect to a server. Usage: connect <server_ip> <server_port>\n"},
    {"Register a node with the server. Usage: register <node_ip> <node_port> <node_info>\n"},
    {"Ping the server to check if it is still active. Usage: ping\n"},
    {"start the terminal, to send commands to the server. Usage: startTerm\n"},
    {"Exit the client terminal. Usage: exit\n"},
};

enum class client_commands_enum {
    connect = 0,
    register_node,
    ping,
    start_terminal,
    exit,
    null,
};

const std::unordered_map<std::string_view, client_commands_enum> str_to_commnd {
    {"connect", client_commands_enum::connect},
    {"register", client_commands_enum::register_node},
    {"ping", client_commands_enum::ping},
    {"start_terminal", client_commands_enum::start_terminal},
    {"exit", client_commands_enum::exit},
};

class LocalTerminal{
    private:
        RemoteTerminal CCLI;

        template <typename C>
        int client_connect(C&& args){
            static_assert(has_iterators<C>::value, "Container must have begin and end iterators");
            static_assert(container_holds_string_like<C>::value, "Container must hold a string_view or string");
            if(args.size() < 3) [[unlikely]] {
                std::cout << "Invalid number of arguments\n";
                std::cout << client_cmd_usage(client_commands_enum::connect);
                return 0;
            }
            if(CCLI.newConnection(*(args.begin() + 1), *(args.begin() + 2))){
                std::cout << "Successful connection to server, terminal and all else available\n";
                return 1;
            } else {
                std::cout << "Failed to connect to server, try again\n";
                return 0;
            }
        }

        template <typename C>
        int client_register(C&& args){
            static_assert(has_iterators<C>::value, "Container must have begin and end iterators");
            static_assert(container_holds_string_like<C>::value, "Container must hold a string_view or string");
            
            if(args.size() < 4) [[unlikely]] {
                std::cout << "Error: Not enough arguments\n";
                std::cout << client_cmd_usage(client_commands_enum::register_node);
                return 0;
            }

            auto begin = args.begin();
            if(CCLI.registerNode(*(begin + 1), *(begin + 2), *(begin + 3))){
                std::cout << "Node registered successfully\n";
                return 1;
            } else {
                std::cout << "Failed to register node\n";
                return 0;
            }
        }

        /**
         * Template parameter there for consistency between other function calls
        */
        template <typename C>
        int client_ping(C&&) {
            if(CCLI.ping()){
                
                std::cout << "Server is active\n";
                return 1;

            } else {
                
                std::cout << "Server is not active\n";
                return 0;
            }
        }

        template <typename C>
        int client_start_terminal(C&&) {return 1;}

        template <typename C>
        int client_exit(C&&) {return 1;} 

        [[nodiscard]] static inline client_commands_enum get_arg_number(const std::string_view& cmd){
            auto it = str_to_commnd.find(cmd);
            if(it == str_to_commnd.end()) return client_commands_enum::null;
            return it->second;
        }

        [[nodiscard]] static inline const std::string_view client_cmd_usage(const client_commands_enum cme) noexcept {
            int arg = static_cast<int>(cme);
            std::cout << arg << '\n';
            if(arg < 0 || arg >= sizeof(client_help) / sizeof (client_help[0])) [[unlikely]]
                return std::string_view("Invalid Command\n");
            return std::string_view(client_help[arg].data());
        }
    
    public:
    
        template <typename C>
        [[nodiscard]] 
        int new_terminal_command(C&& cmd){
            
            static_assert(has_iterators<C>::value, "Container must have begin and end iterators");
            static_assert(container_holds_string_like<C>::value, "Container must hold a string_view or string");
            
            if(cmd.size() < 1) [[unlikely]] return 0;

            client_commands_enum cme = get_arg_number(*(cmd.begin()));
            int result = 0;
            switch(cme){
                case client_commands_enum::connect:
                    std::cout << "Connecting to server\n";
                    result = client_connect(cmd);
                    break;
                case client_commands_enum::register_node:
                    std::cout << "registering to client to server\n";
                    result = client_register(cmd);
                    break;
                case client_commands_enum::ping:
                    result = client_ping(cmd);
                    break;
                case client_commands_enum::start_terminal:
                    result = client_start_terminal(cmd);
                    break;
                case client_commands_enum::exit:
                    result = client_exit(cmd);
                    break;
                default:
                    break;
            }
            return result;
        }

        void start_terminal();

};

