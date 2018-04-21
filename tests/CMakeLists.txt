cmake_minimum_required(VERSION 3.1)

# Required to link with gtest library on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_subdirectory(googletest EXCLUDE_FROM_ALL)

add_executable(tests tests.cpp test_digits.cpp)

set_target_properties(
  tests PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

target_include_directories(tests PRIVATE "../src/")

target_link_libraries(tests gtest_main)

add_test(NAME tests COMMAND tests)

#include(GoogleTest.cmake)
#find_package(GTest)
#gtest_discover_tests(tests)
#gtest_add_tests(tests)
