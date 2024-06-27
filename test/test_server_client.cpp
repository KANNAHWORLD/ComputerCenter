#include <gtest/gtest.h>
#include <thread>
#include "clientLocalTerminal.h"
// #include "centralServer.h"

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

// TEST_F(ServerClient, Successful_Client_Connect) {
//     std::vector<std::string_view> args = {"connect", "localhost", "8080"};
//     std::thread server(start_central_server);
//     sleep(10);
//     EXPECT_EQ(client_connect(args), 0);
// }

