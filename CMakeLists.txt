cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(Ronald LANGUAGES CXX)
# set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_AR           "llvm-ar")
set(CMAKE_LINKER       "lld")
set(CMAKE_NM           "llvm-nm")
set(CMAKE_OBJDUMP      "llvm-objdump")
set(CMAKE_RANLIB       "llvm-ranlib")

set(CMAKE_CXX_FLAGS         "-Wall -Wextra -Wpedantic -Wshadow -Wconversion -pthread -march=native -mavx -mavx2")
# we will use -O2 in debug since the performance will make testing
# mostly unusable without optimization, especially with the sanitizers
set(CMAKE_CXX_FLAGS_DEBUG   "-O2 -g -fsanitize=undefined -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# LTO doesn't work on the lab machines so just use it locally
if (DEFINED ENV{PT_THINLTO_YES})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto=thin")
endif()

set(CMAKE_CXX_LINK_OPTIONS "-pthread")
set(CMAKE_CXX_LINK_OPTIONS_DEBUG "-fsanitize=undefined -fsanitize=address -rdynamic")
set(CMAKE_CXX_LINK_OPTIONS_RELWITHDEBINFO "")
set(CMAKE_CXX_LINK_OPTIONS_RELEASE "")

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_MULTITHREADED OFF)

find_package(Catch2 2.13 REQUIRED)
find_package(Boost COMPONENTS program_options json REQUIRED)

include_directories(inc vendor ${Boost_INCLUDE_DIR})

file(GLOB SOURCES "lib/*.cpp")
file(GLOB RONALD_SOURCES "app/*.cpp")
file(GLOB TEST_SOURCES "tests/*.cpp")

add_executable(ronald ${SOURCES} ${RONALD_SOURCES})
add_executable(tests ${SOURCES} ${TEST_SOURCES})

target_link_libraries(ronald Boost::program_options Boost::json)
target_link_libraries(tests PRIVATE Catch2::Catch2 Boost::program_options Boost::json)

add_definitions(-DDBG_MACRO_NO_WARNING)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DDBG_MACRO_DISABLE)
    add_definitions(-DNDEBUG)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DDEBUG)
    add_definitions(-DDBG_MACRO_DISABLE)
endif()
