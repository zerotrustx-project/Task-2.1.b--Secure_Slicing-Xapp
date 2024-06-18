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
      mdclog_level_set(MDCLOG_DEBUG);
    }
    std::unique_ptr<nexran::Config> config;
    std::unique_ptr<nexran::App> app;
    e2ap::SubscriptionResponse sub_resp;
    e2ap::SubscriptionFailure fail_resp;
    e2ap::SubscriptionDeleteResponse del_resp;
    e2ap::SubscriptionDeleteFailure del_fail;
    e2ap::ControlAck control_ack;
    e2ap::ControlFailure control_fail;
    e2ap::Indication e2ap_ind;
};

TEST_F(NexranTest, SubscriptionResonseTest) {
    bool result = app->handle(&sub_resp);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionFailureTest) {
    bool result = app->handle(&fail_resp);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionDeleteResponseTest) {
    bool result = app->handle(&del_resp);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionControlAckTest) {
    bool result = app->handle(&control_ack);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionControlFailureTest) {
    bool result = app->handle(&control_fail);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, E2apIndicationTest) {
    bool result = app->handle(&e2ap_ind);
    ASSERT_EQ(result, true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
