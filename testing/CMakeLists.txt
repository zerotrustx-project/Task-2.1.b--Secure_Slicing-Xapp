if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Source tree builds are disabled.")
endif()

cmake_minimum_required(VERSION 3.4)
project(NexranTesting)


include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

set(PROJECT_SOURCE_DIR "..")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(ENABLE_BUILD_TIMESTAMP "Enable build timestamp" OFF)

find_package(Threads REQUIRED)
find_package(InfluxDB REQUIRED)
pkg_check_modules(HAVE_MDCLOG REQUIRED mdclog)

find_path(RICXFCPP_INCLUDE_DIRS ricxfcpp/messenger.hpp REQUIRED)
find_library(RICXFCPP_LIBRARIES ricxfcpp REQUIRED)
find_package_handle_standard_args(ricxfcpp DEFAULT_MSG RICXFCPP_LIBRARIES RICXFCPP_INCLUDE_DIRS)
if(NOT ${ricxfcpp_FOUND})
  message(FATAL_ERROR "ricxfcpp not found")
endif()
message(STATUS "found ricxfcpp libs: " ${RICXFCPP_LIBRARIES})
message(STATUS "found ricxfcpp include dirs: " ${RICXFCPP_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-pthread")

find_path(RMR_INCLUDE_DIRS rmr/rmr.h REQUIRED)
find_library(RMR_LIBRARIES rmr_si REQUIRED)
find_package_handle_standard_args(rmr DEFAULT_MSG RMR_LIBRARIES RMR_INCLUDE_DIRS)
if(NOT ${rmr_FOUND})
  message(FATAL_ERROR "rmr not found")
endif()
message(STATUS "found rmr libs: " ${RMR_LIBRARIES})
message(STATUS "found rmr include dirs: " ${RMR_INCLUDE_DIRS})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${PROJECT_SOURCE_DIR}/lib/e2ap/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/e2sm/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# First Test File
add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

# Second Test File
add_executable(
  nexran_test
  nexran_test.cc
)
target_link_libraries(
  nexran_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(nexran_test)
