cmake_minimum_required(VERSION 3.2)

set(PROJECT_LIB milecsa_light)

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

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR} )
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    message(STATUS "Using Clang" )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    message(STATUS "Using gcc" )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=gnu11 -static-libgcc -static-libstdc++ ")

    if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DX86_LINUX")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")

    message( FATAL_ERROR "The project does not support Intel compiler" )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

    message( FATAL_ERROR "The project does not support MSVC compiler" )

endif()

file(GLOB LIBRARY_FILE_LIST ${CMAKE_SOURCE_DIR}/vendor/mile-crypto/*.*)
list(LENGTH LIBRARY_FILE_LIST NUMBER_OF_FILES)
if (NUMBER_OF_FILES EQUAL 0)
    message(STATUS "'mile-crypto' library not found")
    execute_process(COMMAND git submodule update --init --recursive vendor/mile-crypto
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else ()
    message(STATUS "'mile-crypto' library found")
endif()

add_subdirectory(lib)
add_subdirectory(test)
enable_testing ()

set(CMAKE_INSTALL_PREFIX "/usr/local/${PROJECT_LIB}")
