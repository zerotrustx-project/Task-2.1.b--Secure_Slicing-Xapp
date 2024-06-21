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

        e2ap_ind = std::make_unique<e2ap::Indication>();
        e2sm_model = new e2sm::kpm::KpmModel(nullptr);
        kind = new e2sm::kpm::KpmIndication(e2sm_model);
        e2ap_ind->model = new e2sm::kpm::KpmIndication(e2sm_model);


    //   model = new e2sm::kpm::KpmModel(nullptr);
    }

    ~NexranTest() {
        free(kind);
    }
    
    std::unique_ptr<nexran::Config> config;
    std::unique_ptr<nexran::App> app;

    std::unique_ptr<e2ap::Indication> e2ap_ind;
    e2sm::kpm::KpmIndication * kind;
    e2sm::Model * e2sm_model;

};
/*
TEST_F(NexranTest, IndicationKpmModelTest) {
    bool result = app->handle(kind);
    ASSERT_EQ(result, true);
}
*/

TEST_F(NexranTest, SubscriptionResonseTest) {

    e2ap::SubscriptionResponse sub_resp;

    bool result = app->handle(&sub_resp);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionFailureTest) {

    e2ap::SubscriptionFailure fail_resp;

    bool result = app->handle(&fail_resp);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionDeleteResponseTest) {

    e2ap::SubscriptionDeleteResponse del_resp;

    bool result = app->handle(&del_resp);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionDeleteFailTest) {

    e2ap::SubscriptionDeleteFailure del_fail;

    bool result = app->handle(&del_fail);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionControlAckTest) {

    e2ap::ControlAck control_ack;

    bool result = app->handle(&control_ack);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, SubscriptionControlFailureTest) {

    e2ap::ControlFailure control_fail;

    bool result = app->handle(&control_fail);
    ASSERT_EQ(result, true);
}

TEST_F(NexranTest, ErrorIndicationTest) {

    e2ap::ErrorIndication err_ind;

    bool result = app->handle(&err_ind);
    ASSERT_EQ(result, true);
}


// TEST_F(NexranTest, E2apIndicationTest) {
//     bool result = app->handle(e2ap_ind);
//     ASSERT_EQ(result, true);
// }


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
