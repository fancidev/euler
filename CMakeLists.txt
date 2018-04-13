cmake_minimum_required(VERSION 3.1)
project(euler)

##############################################################################
# Executable
##############################################################################

add_executable(euler src/main.cpp 
               src/p1.cpp src/p2.cpp src/p3.cpp src/p4.cpp src/p5.cpp
               src/p6.cpp src/p7.cpp src/p8.cpp src/p9.cpp src/p10.cpp
               src/p11.cpp src/p12.cpp src/p13.cpp src/p14.cpp src/p15.cpp
               src/p16.cpp src/p17.cpp src/p18.cpp
               src/p21.cpp src/p22.cpp src/p23.cpp src/p24.cpp
               src/p26.cpp src/p27.cpp src/p28.cpp src/p29.cpp src/p30.cpp
               src/p31.cpp src/p32.cpp src/p33.cpp src/p34.cpp src/p35.cpp
               src/p36.cpp src/p37.cpp src/p38.cpp src/p39.cpp src/p40.cpp
               src/p41.cpp src/p42.cpp src/p43.cpp src/p44.cpp src/p45.cpp
               src/p46.cpp src/p47.cpp src/p48.cpp src/p49.cpp src/p50.cpp
               src/p51.cpp src/p52.cpp src/p53.cpp src/p54.cpp
               src/p58.cpp src/p59.cpp src/p60.cpp
               src/p61.cpp src/p62.cpp src/p63.cpp src/p64.cpp)

set_target_properties(
  euler PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

# Compiler trait adapted from https://stackoverflow.com/questions/10046114/
# Compiler flags adapted from https://stackoverflow.com/questions/2368811/
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # using Clang
  target_compile_options(euler PUBLIC "-Wall" "-Werror" "-pedantic")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  target_compile_options(euler PUBLIC "-Wall" "-Werror" "-pedantic")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()

endif()

install(TARGETS euler DESTINATION bin)

##############################################################################
# Tests -- run with `make test`
##############################################################################

enable_testing()
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
add_test(p11 euler -r 11)
add_test(p12 euler -r 12)
add_test(p13 euler -r 13)
add_test(p14 euler -r 14)
add_test(p15 euler -r 15)
add_test(p16 euler -r 16)
add_test(p17 euler -r 17)
add_test(p18 euler -r 18)
# ...
add_test(p21 euler -r 21)
add_test(p22 euler -r 22)
add_test(p23 euler -r 23)
add_test(p24 euler -r 24)
# ...
add_test(p26 euler -r 26)
add_test(p27 euler -r 27)
add_test(p28 euler -r 28)
add_test(p29 euler -r 29)
add_test(p30 euler -r 30)
add_test(p31 euler -r 31)
add_test(p32 euler -r 32)
add_test(p33 euler -r 33)
add_test(p34 euler -r 34)
add_test(p35 euler -r 35)
add_test(p36 euler -r 36)
add_test(p37 euler -r 37)
add_test(p38 euler -r 38)
add_test(p39 euler -r 39)
add_test(p40 euler -r 40)
add_test(p41 euler -r 41)
add_test(p42 euler -r 42)
add_test(p43 euler -r 43)
add_test(p44 euler -r 44)
add_test(p45 euler -r 45)
add_test(p46 euler -r 46)
add_test(p47 euler -r 47)
add_test(p48 euler -r 48)
add_test(p49 euler -r 49)
add_test(p50 euler -r 50)
add_test(p51 euler -r 51)
add_test(p52 euler -r 52)
add_test(p53 euler -r 53)
add_test(p54 euler -r 54)
# ...
add_test(p58 euler -r 58)
add_test(p59 euler -r 59)
add_test(p60 euler -r 60)
add_test(p61 euler -r 61)
add_test(p62 euler -r 62)
add_test(p63 euler -r 63)
add_test(p64 euler -r 64)

##############################################################################
# Notes -- run with `make pdf`
##############################################################################

# Enable target only if pdflatex is available
find_package(LATEX COMPONENTS PDFLATEX)
if(${LATEX_PDFLATEX_FOUND})
  add_subdirectory(notes EXCLUDE_FROM_ALL)
endif()

##############################################################################
# Documentation -- run with `make doc`
##############################################################################

# Enable target only if doxygen is available
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/Doxyfile.in 
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
                 @ONLY)
  add_custom_target(doc
                    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating documentation with Doxygen" VERBATIM)
endif()
