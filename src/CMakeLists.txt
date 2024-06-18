include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip                                                                                             )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  nexran
  policy.cc nodeb.cc ue.cc slice.cc restserver.cc
  config.cc app_config.cc nexran.cc main.cc buildinfo.cc)
target_link_libraries(nexran e2ap e2sm pistache_shared mdclog ricxfcpp rmr_si InfluxData::InfluxDB)

add_executable(
  nexran_test
  nexran_test.cc policy.cc nodeb.cc ue.cc slice.cc restserver.cc
    config.cc app_config.cc nexran.cc buildinfo.cc
)
target_link_libraries(
  nexran_test
  GTest::gtest_main GTest::gmock_main
  e2ap e2sm pistache_shared mdclog ricxfcpp rmr_si InfluxData::InfluxDB     )

include(GoogleTest)
gtest_discover_tests(nexran_test)

install(TARGETS nexran DESTINATION bin)
