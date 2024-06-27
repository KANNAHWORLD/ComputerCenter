#include <unordered_map>
#include "clientRemoteTerminal.h"
#include "typeCheck.h"

const std::string client_help[] = {
    {"Connect to a server. Usage: connect <server_ip> <server_port>\n"},
    {"Register a node with the server. Usage: register <node_ip> <node_port> <node_info>\n"},
    {"Ping the server to check if it is still active. Usage: ping\n"},
    {"start the terminal, to send commands to the server. Usage: startTerm\n"},
    {"Exit the client terminal. Usage: exit\n"},
};

const enum class client_commands_enum {
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
        int client_connect(C&&);

        template <typename C>
        int client_register(C&&);

        template <typename C>
        int client_ping(C&&) {return 1;}

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
            if(arg < 0 || arg >= sizeof(client_help) / sizeof (client_help[0])) [[unlikely]]
                return std::string_view("Invalid Command\n");
            return std::string_view(client_help[arg].data());
        }
    
    public:
    
        [[nodiscard]] int new_terminal_command(const std::vector<std::string_view>&);

        void start_terminal();

};

