cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(euler)

add_executable(euler src/main.cpp 
               src/p1.cpp src/p2.cpp src/p3.cpp src/p4.cpp src/p5.cpp
               src/p6.cpp src/p7.cpp src/p8.cpp src/p9.cpp src/p10.cpp)

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
add_test(p2 euler -r 2)
add_test(p3 euler -r 3)
add_test(p4 euler -r 4)
add_test(p5 euler -r 5)
add_test(p6 euler -r 6)
add_test(p7 euler -r 7)
add_test(p8 euler -r 8)
add_test(p9 euler -r 9)
add_test(p10 euler -r 10)
