#include <unordered_map>
#include <functional>

int client_connect(const std::vector<std::string_view>&);
int client_register(const std::vector<std::string_view>&);
int client_ping(const std::vector<std::string_view>&);
int client_exit(const std::vector<std::string_view>&);
int client_start_terminal(const std::vector<std::string_view>&);

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
};

const std::unordered_map<client_commands_enum, std::function<int(const std::vector<std::string_view>&)>> client_commands = {
    {client_commands_enum::connect, client_connect},
    {client_commands_enum::register_node, client_register},
    {client_commands_enum::ping, client_ping},
    {client_commands_enum::start_terminal, client_start_terminal},
    {client_commands_enum::exit, client_exit},
};


[[nodiscard, always_inline]] 
inline const std::string_view client_cmd_usage(const client_commands_enum cme) noexcept {
    int arg = static_cast<int>(cme);
    if(arg < 0 || arg >= sizeof(client_help) / sizeof (client_help[0])) [[unlikely]]
        return std::string_view("Invalid Command\n");
    return std::string_view(client_help[arg].data());
}

