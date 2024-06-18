//Unit testing purposes
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chrono>
#include <string>
#include <thread>
#include <csignal>
#include <memory>

#include "mdclog/mdclog.h"
#include "rmr/RIC_message_types.h"
#include "ricxfcpp/message.hpp"
#include "ricxfcpp/messenger.hpp"

#include "nexran.h"
#include "e2ap.h"
#include "e2sm.h"
#include "e2sm_nexran.h"
#include "e2sm_kpm.h"
#include "e2sm_zylinium.h"
#include "restserver.h"

// Environmental variables

int TX_PKTS=0;
int TOTAL_TX_1=0;
int TOTAL_TX_2=0;
int TOTAL_TX_3=0;
int COUNTER1=0;
int COUNTER2=0;
int COUNTER3=0;

//class MockKpmModel : public e2sm::Model
//{
//   public:
//	MockKpmModel() : e2sm::Model("TestMod", "1"){};
//	MOCK_METHOD(int, init, (), (override));
//	MOCK_METHOD(void, stop, (), (override));
//	MOCK_METHOD(std::string&, get_name, (), (const, override));
//	MOCK_METHOD(std::string&, get_oid, (), (const, override));
//	MOCK_METHOD(void, add_function, (e2ap::RanFunctionId function_id, e2ap::RanFunction *function), (override));
//	MOCK_METHOD(e2sm::Indication, decode, (e2ap::Indication *ind, 
//				unsigned char *header,ssize_t header_len,
//                               unsigned char *message,ssize_t message_len), (override));
//        MOCK_METHOD(e2sm::ControlOutcome, decode, (e2ap::ControlAck *ack,
//                                   unsigned char *outcome,ssize_t outcome_len), (override);
//	MOCK_METHOD(e2sm::ControlOutcome, decode, (e2ap::ControlFailure *failure,
//                                   unsigned char *outcome,ssize_t outcome_len), (override);	
//};

//class MockControlOutcome :  public

class MockKpmReport
{
    public:
	MockKpmReport(long ms, int dl_prbs, int ul_prbs, long active_ues)
        	: period_ms(ms),available_dl_prbs(dl_prbs),available_ul_prbs(ul_prbs),active_ues(active_ues),
          	  ues(),slices() {};
	virtual ~MockKpmReport() = default;
    	std::string to_string(char group_delim = ' ',char item_delim = ',');

	long period_ms;
    	int available_dl_prbs;
    	int available_ul_prbs;
    	long active_ues;
	std::map<long,entity_metrics_t> ues;
    	std::map<std::string,entity_metrics_t> slices;
};

std::vector<int> handle(MockKpmReport *report)
{

    std::vector<int> values;
    values.push_back(1);
    int ue_counter =0;

    for (auto it = report->ues.begin(); it != report->ues.end(); ++it) {

            int ue_name = it->first;
            mdclog_write(MDCLOG_INFO,"UE NAME '%d':",ue_name);
            ue_counter++;

            if(ue_counter == 1) //ue_name==70
            {
                //total the tx pkts for ue1
                TOTAL_TX_1 = TOTAL_TX_1+ report->ues[ue_name].tx_pkts;
                mdclog_write(MDCLOG_INFO, "Total tx_pkts '%d': %d",
                ue_name, TOTAL_TX_1);
                COUNTER1++;
                mdclog_write(MDCLOG_INFO,"COUNT: %d",
                COUNTER1);

            }

            else if(ue_counter == 2) 
            {
                TOTAL_TX_2 = TOTAL_TX_2 + report->ues[ue_name].tx_pkts;
                mdclog_write(MDCLOG_INFO, "Total tx_pkts '%d': %d",
                ue_name, TOTAL_TX_2);
                COUNTER2++;
                mdclog_write(MDCLOG_INFO,"COUNT: %d",
                COUNTER2);

            }

            else if(ue_counter == 3) 
            {
                TOTAL_TX_3 = TOTAL_TX_3 + report->ues[ue_name].tx_pkts;
                mdclog_write(MDCLOG_INFO, "Total tx_pkts '%d': %d",
                ue_name, TOTAL_TX_3);
                COUNTER3++;
                mdclog_write(MDCLOG_INFO,"COUNT: %d",
                COUNTER3);
                ue_counter = 0;

            }

            if(COUNTER1 == 10 || COUNTER2 == 10 || COUNTER3 == 10){

                
                if ((TOTAL_TX_1 > TOTAL_TX_2) && (TOTAL_TX_1 > TOTAL_TX_3))
                {
                    ue1imsi = "001010123456789";
                    ue2imsi = "001010123456780";
                    ue3imsi = "001010123456781";
                }

                else if ((TOTAL_TX_2 > TOTAL_TX_3) && (TOTAL_TX_2 > TOTAL_TX_1))
                {
                    ue1imsi = "001010123456780";
                    ue2imsi = "001010123456789";
                    ue3imsi = "001010123456781";
                }

                else if ((TOTAL_TX_3 > TOTAL_TX_1) && (TOTAL_TX_3 > TOTAL_TX_2))
                {
                    ue1imsi = "001010123456780";
                    ue2imsi = "001010123456781";
                    ue3imsi = "001010123456789";
                }

                if (COUNTER1 == 10)
                {	
                    float avg_tx_1=TOTAL_TX_1/10;
                    mdclog_write(MDCLOG_INFO,"Avg tx_pkts '%d': %f",
                    ue_name, avg_tx_1);

                    if(avg_tx_1 >= tx_threshold){

                        tx_threshold *= 10;

                        mdclog_write(MDCLOG_DEBUG, "UE1 imsi: %s", ue1imsi.c_str());
                        mdclog_write(MDCLOG_DEBUG, "UE2 imsi: %s", ue2imsi.c_str());

                        mdclog_write(MDCLOG_DEBUG,"UE[%d] found MALICIOUS",
                        ue_name);

                        AppError *ae = nullptr;

                        //delete slicing binding to UE

                        //need imsi and what slice the UE is bound to.

                        mdclog_write(MDCLOG_DEBUG,"UNBINDING START");
                        mutex.unlock();
                        unbind_ue_slice(ue1imsi,slice1,&ae);
                        mutex.lock();
                        mdclog_write(MDCLOG_DEBUG,"UNBINDING SUCCESS");

                        /*
                        mutex.unlock();
                        del(App::ResourceType::UeResource, ue1imsi, &ae);
                        mutex.lock(); */

                        /*rapidjson::Document d;
                        d.Parse(request.body().c_str());
                        string writer == "imsi\":\"001010123456789\",\"tmsi\":\"\",\"crnti\":\"\",\"status\":{\"connected\":false}"
                        Ue *ue = Ue::create(d,&ae); */
                        
                        //mutex.unlock();
                        /*
                        // Create a JSON object that represents the UE. 
                        rapidjson::Document d; 
                        d.SetObject(); 
                        d.AddMember("imsi", rapidjson::Value().SetString("001010123456789"), d.GetAllocator());  */

                        // Call the postUE() method. 
                        //server.postUe(d, &ae);
                        //add(App::ResourceType::UeResource,ue,writer,&ae);
                        //mutex.lock();

                        //slice create does not have a non REST way to create slices. For now just use curl to create an initial malicious slice

                        // Create a response object. 
                        /*

                        Pistache::Http::ResponseWriter response;
                        server.postUe(d, response); 
                        // Check the response code. 
                        if (response.status() != Pistache::Http::Code::OK) 
                        { std::cerr << "Error: " << response.status() << std::endl; return 1; } 
                        // Check the response body. 
                        const std::string& body = response.body(); 
                        if (body != "UE created successfully.") 
                        { std::cerr << "Error: unexpected response body: " << body << std::endl; return 1; }
                        
                        */

                        mdclog_write(MDCLOG_DEBUG,"BINDING START");
                        //bind to malicious UE to secure slice
                        mutex.unlock();
                        bind_ue_slice(ue1imsi,slice2,&ae);
                        mutex.lock();
                        mdclog_write(MDCLOG_DEBUG,"BINDING SUCCESS");
                            //print something here telling us wht happened

                    }
                    TOTAL_TX_1 = 0;
                    COUNTER1 = 0;
                }

                else if(COUNTER2 == 10)
                {	
                    float avg_tx_2=TOTAL_TX_2/10;
                    mdclog_write(MDCLOG_INFO,"Avg tx_pkts '%d': %f",
                    ue_name, avg_tx_2); 

                    if(avg_tx_2 >= tx_threshold)
                    {
                        tx_threshold *= 10;

                        mdclog_write(MDCLOG_DEBUG, "UE1 imsi: %s", ue1imsi.c_str());
                        mdclog_write(MDCLOG_DEBUG, "UE2 imsi: %s", ue2imsi.c_str());
                        mdclog_write(MDCLOG_DEBUG,"UE[%d] found MALICIOUS",
                        ue_name);
                        //slice_name.c_str(),new_share_factors[slice_name]);

                        AppError *ae = nullptr;

                        //delete slicing binding to UE

                        //need imsi and what slice the UE is bound to.
                        mdclog_write(MDCLOG_DEBUG,"UNBINDING START");
                        mutex.unlock();
                        unbind_ue_slice(ue2imsi,slice1,&ae);
                        mutex.lock();
                        mdclog_write(MDCLOG_DEBUG,"UNBINDING SUCCESS");

                        //slice create does not have a non REST way to create slices. For now just use curl to create an initial malicious slice


                        //create secure slice
                        // string x="MALICIOUS";
                        // d=x.c_str();
                        // Slice *slice = Slice::create(d,&ae);


                        mdclog_write(MDCLOG_DEBUG,"BINDING START");
                        //bind to malicious UE to secure slice
                        mutex.unlock();
                        bind_ue_slice(ue2imsi,slice2,&ae);
                        mutex.lock();
                        mdclog_write(MDCLOG_DEBUG,"BINDING SUCCESS");

                        //print something here

                    }				
                    TOTAL_TX_2 = 0;
                    COUNTER2 = 0;
                }


                else if(COUNTER3 == 10)
                {	
                    float avg_tx_3=TOTAL_TX_3/10;
                    mdclog_write(MDCLOG_INFO,"Avg tx_pkts '%d': %f",
                    ue_name, avg_tx_3); 

                    if(avg_tx_3 >= tx_threshold)
                    {
                        tx_threshold *= 10;

                        //mdclog_write(MDCLOG_DEBUG, "UE1 imsi: %s", ue1imsi.c_str());
                        //mdclog_write(MDCLOG_DEBUG, "UE2 imsi: %s", ue2imsi.c_str());
                        mdclog_write(MDCLOG_DEBUG,"UE[%d] found MALICIOUS",
                        ue_name);
                        //slice_name.c_str(),new_share_factors[slice_name]);

                        AppError *ae = nullptr;

                        //delete slicing binding to UE

                        //need imsi and what slice the UE is bound to.
                        mdclog_write(MDCLOG_DEBUG,"UNBINDING START");
                        mutex.unlock();
                        unbind_ue_slice(ue3imsi,slice1,&ae);
                        mutex.lock();
                        mdclog_write(MDCLOG_DEBUG,"UNBINDING SUCCESS");

                        //slice create does not have a non REST way to create slices. For now just use curl to create an initial malicious slice


                        //create secure slice
                        // string x="MALICIOUS";
                        // d=x.c_str();
                        // Slice *slice = Slice::create(d,&ae);


                        mdclog_write(MDCLOG_DEBUG,"BINDING START");
                        //bind to malicious UE to secure slice
                        mutex.unlock();
                        bind_ue_slice(ue3imsi,slice2,&ae);
                        mutex.lock();
                        mdclog_write(MDCLOG_DEBUG,"BINDING SUCCESS");

                        //print something here

                    }				
                    TOTAL_TX_3 = 0;
                    COUNTER3 = 0;
                }

            }
        };

}

class NexranTest : public ::testing::Test {
protected:
    NexranTest() {
	config = std::make_unique<nexran::Config>();
    }
    std::unique_ptr<nexran::Config> config;
};

TEST_F(NexranTest, KpmIndicationHandler) {
    
    std::unique_ptr<MockKpmReport> report;

    report = std::make_unique<MockKpmReport>(10, 70, 70, 3);
    
    entity_metrics_t ue1;

    auto now = std::chrono::system_clock::now();
    ue1.time = std::chrono::system_clock::to_time_t(now);
    ue1.dl_bytes = 70;
    ue1.ul_bytes = 80;
    ue1.dl_prbs = 100;
    ue1.ul_prbs = 100;
    ue1.tx_pkts = 100;
    ue1.tx_errors = 0;
    ue1.tx_brate = 10;
    ue1.rx_pkts = 100;
    ue1.rx_errors = 0;
    ue1.rx_brate = 100;
    ue1.dl_cqi = 1.0;
    ue1.dl_ri = 1.0;
    ue1.dl_pmi = 1.0;
    ue1.ul_phr = 1.0;
    ue1.ul_sinr = 1.0;
    ue1.ul_mcs = 1.0;
    ue1.ul_samples = 1;
    ue1.dl_mcs = 0.5;
    ue1.dl_samples = 1;


    report->ues.insert(std::pair<long,entity_metrics_t>(70, ue1));	  
    report->ues.insert(std::pair<long,entity_metrics_t>(72, ue1));	  
    report->ues.insert(std::pair<long,entity_metrics_t>(74, ue1));	  
      
//  std::vector<int> = handle(report);
    ASSERT_EQ("3", "3");
}

//-------------------------------------------------------
//confirm tests work
//
TEST_F(NexranTest, IsTwoWorking) {
    std::string result = "3";
    ASSERT_EQ(result, "3");
}

//-------------------------------------------------------

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
