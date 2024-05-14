#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <csignal>
#include <memory>


#include "mdclog/mdclog.h"
#include "nexran.h"
#include "restserver.h"


class NexranTest : public ::testing::Test {
protected:
    NexranTest() {
      a = 3;
      b = 5;
      config = std::make_unique<nexran::Config>();
    }
    int a;
    int b;
    std::unique_ptr<nexran::Config> config;
    std::unique_ptr<nexran::App> app;
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
