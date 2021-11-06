cmake_minimum_required(VERSION 3.1)

project(stock C CXX)


##
### Settings ###
##
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_COLOR_MAKEFILE   ON)
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON) # disable in source builds
SET(CMAKE_INSTALL_PREFIX /srv/stock)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0 -pipe -Wall -Wunknown-pragmas -L/usr/lib -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -pipe -fomit-frame-pointer -Wunknown-pragmas -L/usr/lib -Wno-write-strings")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_CPPLINT "cpplint") # attach CPPLINT
set(CMAKE_CXX_CPPCHECK "cppcheck") # attach CPPCHECK
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE RelWithDebInfo)


##
### CPPCHECK ###
##
find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list(
        APPEND CMAKE_CXX_CPPCHECK 
            "--enable=warning"
            "--inconclusive"
            "--force" 
            "--inline-suppr"
            "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )
endif()

##
### Options ###
##
## Testing
option(ENABLE_TESTS "Build the tests" ON)
if(ENABLE_TESTS)
    message(STATUS "ENABLE_TESTS: " ${ENABLE_TESTS})
    enable_testing()
    include(cmake/GoogleTest.cmake)
    add_subdirectory(test)
endif()

##
### Source definitions ###
##
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB sources
  "${PROJECT_SOURCE_DIR}/include/stock/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(stock ${sources})