cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 11)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)

set (BOOST_COMPONENTS)
find_package (Boost  REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(PROJECT build-transaction-example)

find_package(milecsa_light CONFIG REQUIRED)

include_directories(
        ${milecsa_light_INCLUDE_PATH}
)

add_executable(${PROJECT} build-transaction.cpp)
target_link_libraries(${PROJECT} milecsa_light)