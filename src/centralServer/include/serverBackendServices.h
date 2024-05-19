/**
 * @file serverBackendServices.h
 * @briefs Contains the declarations for functions to serve RPCs.
*/

/**
 * @brief Loads the nodes from the file system.
 * @brief Stores the nodes into the file system.
 * Loads during creation of the server.
 * Saves at shutdown of the server.
*/
void load_nodes();
void save_nodes();


/**
 * @brief Services offered by RPCs
*/

// Add a new compute node to the system
bool add_new_compute_node(const std::string&, const std::string&, const std::string&) noexcept;

// Retrieve all compute nodes in the system
std::string retrieve_all_nodes();

// Run a command on the system
std::string run_system_command(std::string);