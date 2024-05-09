cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(mod STATIC mod.cc)

target_include_directories(mod PUBLIC mod.h)

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
  second_test
  second_test.cc
)
target_link_libraries(
  second_test
  GTest::gtest_main
  mod
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(second_test)
