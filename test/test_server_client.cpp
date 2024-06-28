#include <gtest/gtest.h>
#include <thread>
#include "clientLocalTerminal.h"
#include <csignal>
#include "centralServer.h"

#define SERVER_WAIT_TIME 2

// Define your test fixture
class ServerClient : public ::testing::Test {
private:
protected:
    // Set up the test fixture
    void SetUp() override {
        // Re-initialize the client terminal
    }

    // Tear down the test fixture
    void TearDown() override {
    }
};

// Define your test cases
TEST_F(ServerClient, Client_Connect_Failed_Not_Enough_Args) {
    std::vector<std::string_view> args = {"connect"};
    LocalTerminal clientTerm;
    int result = clientTerm.new_terminal_command(args);
    EXPECT_EQ(result, 0);
}

TEST_F(ServerClient, Client_Connect_Failed_Server_Down) {
    std::vector<std::string_view> args = {"connect", "localhost", "8080"};
    LocalTerminal clientTerm;
    int result = clientTerm.new_terminal_command(args);
    EXPECT_EQ(result, 0);
}

TEST_F(ServerClient, unsuccessful_Client_Ping_no_connection) {
    
    std::vector<std::string_view> ping_args = {"ping"};
    LocalTerminal clientTerm;
    EXPECT_EQ(clientTerm.new_terminal_command(ping_args), 0);
}

TEST_F(ServerClient, unsuccessful_Client_Register_no_connection) {
    
    std::vector<std::string_view> register_args = {"register", "localhost", "8080", "Sid's Macbook Test"};
    LocalTerminal clientTerm;
    EXPECT_EQ(clientTerm.new_terminal_command(register_args), 0);
}

TEST_F(ServerClient, unsuccessful_Client_Register_not_enough_args) {
    
    std::vector<std::string_view> register_args = {"register", "localhost", "Sid's Macbook Test"};
    LocalTerminal clientTerm;
    EXPECT_EQ(clientTerm.new_terminal_command(register_args), 0);
}

#ifndef WIN32
TEST_F(ServerClient, Successful_Client_Connect) {
    std::vector<std::string_view> args = {"connect", "localhost", "8080"};
    LocalTerminal clientTerm;
    pid_t pid;
    
    pid = fork();

    if(pid == 0){
        // Essentially, we do not care about the output of the server, just want to send commands to shwo that it does connect
        // and that the server is running
        close(STDOUT_FILENO);
        // Starts a new program running the server
        execl("/bin/sh", "sh", "-c", "./build/main", (char *)NULL);
    } 

    sleep(SERVER_WAIT_TIME);
    EXPECT_EQ(clientTerm.new_terminal_command(args), 1);
    kill(pid, SIGKILL);
}

TEST_F(ServerClient, Successful_Client_Ping) {
    std::vector<std::string_view> connect_args = {"connect", "localhost", "8080"};
    std::vector<std::string_view> ping_args = {"ping"};

    LocalTerminal clientTerm;
    pid_t pid;
    
    pid = fork();

    if(pid == 0){
        // Essentially, we do not care about the output of the server, just want to send commands to shwo that it does connect
        // and that the server is running
        close(STDOUT_FILENO);
        // Starts a new program running the server
        execl("/bin/sh", "sh", "-c", "./build/main", (char *)NULL);
    } 

    sleep(SERVER_WAIT_TIME);
    EXPECT_EQ(clientTerm.new_terminal_command(connect_args), 1);
    EXPECT_EQ(clientTerm.new_terminal_command(ping_args), 1);
    kill(pid, SIGKILL);
}

TEST_F(ServerClient, Successful_Client_Register) {
    std::vector<std::string_view> connect_args = {"connect", "localhost", "8080"};
    std::vector<std::string_view> register_args = {"register", "localhost", "8080", "Sid's Macbook Test"};
    LocalTerminal clientTerm;
    pid_t pid;
    
    pid = fork();

    if(pid == 0){
        // Essentially, we do not care about the output of the server, just want to send commands to shwo that it does connect
        // and that the server is running
        close(STDOUT_FILENO);
        // Starts a new program running the server
        execl("/bin/sh", "sh", "-c", "./build/main", (char *)NULL);
    } 

    sleep(SERVER_WAIT_TIME);
    EXPECT_EQ(clientTerm.new_terminal_command(connect_args), 1);
    EXPECT_EQ(clientTerm.new_terminal_command(register_args), 1);
    kill(pid, SIGKILL);
}
#endif
