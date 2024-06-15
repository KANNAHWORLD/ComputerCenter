#include <gtest/gtest.h>

// Define your test fixture
class UtilsTests : public ::testing::Test {
protected:
    // Set up the test fixture
    void SetUp() override {
        // Add any necessary setup code here
    }

    // Tear down the test fixture
    void TearDown() override {
        // Add any necessary teardown code here
    }
};

// Define your test cases
TEST_F(UtilsTests, TestName) {
    // Add your test code here
    ASSERT_TRUE(true);
}
