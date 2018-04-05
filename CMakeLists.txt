cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(euler)

add_executable(euler src/main.cpp src/p1.cpp)

# Adapted from https://stackoverflow.com/questions/2368811/
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
else()
  # Update if necessary
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
  target_compile_options(euler PUBLIC "-Wall" "-Werror" "-pedantic")
endif()

install(TARGETS euler DESTINATION bin)

# Tests
include(CTest)
add_test(p1 euler -r 1)
