#include <gtest/gtest.h>
#include <string>
#include "nexran.h"


class NexranTest : public ::testing::Test {
protected:
    NexranTest() {
      a = 3;
      b = 5;
    }

    int a;
    int b;
};

TEST_F(NexranTest, IsOneWorking) {
    int result = a + b;
    ASSERT_EQ(result, 8);
}

TEST_F(NexranTest, IsTwoWorking) {
    std::string result = "3";
    ASSERT_EQ(result, "3");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
