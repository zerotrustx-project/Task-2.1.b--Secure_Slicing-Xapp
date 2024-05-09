#include <gtest/gtest.h>
#include <string>
#include "mod.h"


class TestingTest : public ::testing::Test {
protected:
    TestingTest() {
      a = 3;
      b = 5;
    }

    int a;
    int b;
};

TEST_F(TestingTest, IsOneWorking) {
    int result = one(a, b);
    ASSERT_EQ(result, 8);
}

TEST_F(TestingTest, IsTwoWorking) {
    std::string result = two(a);
    ASSERT_EQ(result, "3");
}

// Add more test cases as needed

// Define the main function to run the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
