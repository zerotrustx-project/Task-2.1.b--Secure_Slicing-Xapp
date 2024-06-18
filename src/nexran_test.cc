#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <csignal>
#include <memory>


#include "mdclog/mdclog.h"
#include "nexran.h"
#include "restserver.h"
#include "e2ap.h"
#include "e2sm.h"



class NexranTest : public ::testing::Test {
protected:
    NexranTest() {
      config = std::make_unique<nexran::Config>();
      app = std::make_unique<nexran::App>(*config);
    }
    std::unique_ptr<nexran::Config> config;
    std::unique_ptr<nexran::App> app;
    e2ap::SubscriptionResponse resp;
};

TEST_F(NexranTest, KpmHandle) {
    bool result = app->handle(&resp);
    ASSERT_EQ(result, true);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
