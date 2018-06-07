cmake_minimum_required(VERSION 3.1)
project(euler)

##############################################################################
# Executable
##############################################################################

set(SolutionNumbers
    1   2   3   4   5   6   7   8   9  10
   11  12  13  14  15  16  17  18  19  20
   21  22  23  24  25  26  27  28  29  30
   31  32  33  34  35  36  37  38  39  40
   41  42  43  44  45  46  47  48  49  50
   51  52  53  54              58  59  60
   61  62  63  64      66  67  68  69  70
   71  72  73  74  75  76  77  78
   81  82  83  84                  89
       92      94      96  97  98  99 100
  101
                                  239
                          307
      622     624
  )

set(SolutionFiles "")
foreach(SolutionNumber ${SolutionNumbers})
  set(SolutionFiles ${SolutionFiles} "src/p${SolutionNumber}.cpp")
endforeach()
# message(STATUS "SolutionFiles: ${SolutionFiles}")

add_executable(euler src/main.cpp ${SolutionFiles})

set_target_properties(
  euler PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)

# Compiler trait adapted from https://stackoverflow.com/questions/10046114/
# Compiler flags adapted from https://stackoverflow.com/questions/2368811/
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # using Clang
  target_compile_options(euler PUBLIC
    "-Wall" "-Werror" "-pedantic" "-Wdocumentation" "-Wextra"
  )

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  target_compile_options(euler PUBLIC "-Wall" "-Werror" "-pedantic")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
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
foreach(SolutionNumber ${SolutionNumbers})
  add_test("p${SolutionNumber}" euler -r ${SolutionNumber})
endforeach()

##############################################################################
# Lint -- run with `make tidy`
##############################################################################

# Enable clang-tidy if the compiler selected is clang.
# References:
# http://www.parresianz.com/c++/clang-tidy/
# http://mariobadr.com/using-clang-tidy-with-cmake-36.html
# https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # Generate compile_commands.json (required by clang-tidy)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  find_program(CLANG_TIDY_EXE clang-tidy DOC "clang-tidy")
  if(NOT CLANG_TIDY_EXE)
    message(STATUS "Omitting target 'tidy' because clang-tidy is not found")
  else()
    set(TidyTargets "")
    foreach(SolutionNumber ${SolutionNumbers})
      set(TidyTarget "tidy-p${SolutionNumber}")
      add_custom_target("${TidyTarget}"
        COMMAND "${CLANG_TIDY_EXE}"
        "-p" "${CMAKE_CURRENT_BINARY_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/p${SolutionNumber}.cpp"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/p${SolutionNumber}.cpp"
        COMMENT "Running clang-tidy on p${SolutionNumber}.cpp"
      )
      set(TidyTargets ${TidyTargets} ${TidyTarget})
    endforeach()
    add_custom_target(tidy DEPENDS ${TidyTargets})
  endif()

  # Run clang-tidy as part of the build process
  if(CLANG_TIDY_EXE)
    set_target_properties(euler PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
endif()

##############################################################################
# Library Unit Tests -- run with `make test`
##############################################################################

add_subdirectory(tests)

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
