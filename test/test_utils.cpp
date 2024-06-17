#include <gtest/gtest.h>
#include "stringOps.h"

// Define your test fixture
class UtilsTests : public ::testing::Test {
protected:
    // Set up the test fixture
    void SetUp() override {
    }

    // Tear down the test fixture
    void TearDown() override {
    }
};

// Define your test cases
TEST_F(UtilsTests, StringSplitReturnVec) {
    std::string test_string = "Hello World";
    std::vector<std::string_view> result = split_string(test_string, ' ');
    std::vector<std::string_view> expected = {"Hello", "World"};
    ASSERT_EQ(result, expected);
}

TEST_F(UtilsTests, StringSplitReturnVecEmpty) {
    std::string test_string = "";
    std::vector<std::string_view> result = split_string(test_string, ' ');
    ASSERT_EQ(result.size(), 0);
}

TEST_F(UtilsTests, StringSplitReturnVecEmptySubtring) {
    std::string test_string = "Hello   World";
    std::vector<std::string_view> result = split_string(test_string, ' ');
    std::vector<std::string_view> expected = {"Hello", "World"};
    ASSERT_EQ(result, expected);
}

TEST_F(UtilsTests, StringSplitReferenceVec) {
    std::string test_string = "Hello World";
    std::vector<std::string_view> result;
    split_string(test_string, result, ' ');
    std::vector<std::string_view> expected = {"Hello", "World"};
    ASSERT_EQ(result, expected);
}

TEST_F(UtilsTests, StringSplitReferenceVecEmpty) {
    std::string test_string = "";
    std::vector<std::string_view> result;
    split_string(test_string, result, ' ');
    ASSERT_EQ(result.size(), 0);
}

TEST_F(UtilsTests, StringSplitReferenceVecEmptySubtrings) {
    std::string test_string = "Hello     World";
    std::vector<std::string_view> result;
    split_string(test_string, result, ' ');
    std::vector<std::string_view> expected = {"Hello", "World"};
    ASSERT_EQ(result, expected);
}

TEST_F(UtilsTests, StringSplitReferenceVecResultContainsPrevData) {
    std::string test_string = "Hello     World";
    std::vector<std::string_view> result;
    result.emplace_back(test_string);
    split_string(test_string, result, ' ');
    std::vector<std::string_view> expected = {"Hello", "World"};
    ASSERT_EQ(result, expected);
}

TEST_F(UtilsTests, StringSplitInefficientReturnVec) {
    std::string test_string = "Hello     World";
    std::vector<std::string> result;
    result = split_string_inefficient(test_string, ' ');
    std::vector<std::string> expected = {"Hello", "World"};
    ASSERT_EQ(result, expected);
}

