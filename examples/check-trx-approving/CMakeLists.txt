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

set(PROJECT send-1xdr-wait-for-approving)

find_package(milecsa_light CONFIG REQUIRED)
find_package(CURL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        ${milecsa_light_INCLUDE_PATH}
        ../../vendor/nlohmann
)

if (${CURL_FOUND})
    message(STATUS "curl library version: " ${CURL_VERSION_STRING})
    add_compile_definitions(WITH_CURL)
endif ()

add_executable(${PROJECT} send-1xdr-wait-for-approving.cpp)
target_link_libraries(
        ${PROJECT}
        milecsa_light
        curl
)
