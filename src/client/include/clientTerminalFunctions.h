#include <unordered_map>
#include <functional>

int client_connect();
int client_register();
int client_ping();
int client_exit();
int client_start_terminal();

const std::unordered_map<std::string, std::function<int()>> client_commands = {
    {"connect", client_connect},
    {"register", client_register},
    {"ping", client_ping},
    {"startTerm", client_start_terminal},
    {"exit", client_exit},
};

const std::unordered_map<std::string, std::string> client_help = {
    {"connect", "Connect to a server. Usage: connect <server_ip> <server_port>"},
    {"register", "Register a node with the server. Usage: register <node_ip> <node_port> <node_info>"},
    {"ping", "Ping the server to check if it is still active. Usage: ping"},
    {"startTerm", "start the terminal, to send commands to the server. Usage: startTerm"},
    {"exit", "Exit the client terminal. Usage: exit"},
};
