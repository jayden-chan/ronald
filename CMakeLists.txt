cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(path_tracer LANGUAGES CXX)
# set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_AR           "/usr/bin/llvm-ar")
set(CMAKE_LINKER       "/usr/bin/lld")
set(CMAKE_NM           "/usr/bin/llvm-nm")
set(CMAKE_OBJDUMP      "/usr/bin/llvm-objdump")
set(CMAKE_RANLIB       "/usr/bin/llvm-ranlib")

set(CMAKE_CXX_FLAGS         "-Wall -Wextra -Wpedantic -Wshadow -Wconversion")
# we will use the -O3 flag in debug since the performance will make testing
# mostly unusable without optimization
set(CMAKE_CXX_FLAGS_DEBUG   "-O3 -g -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto=thin -DNDEBUG")

set(CMAKE_CXX_LINK_OPTIONS_DEBUG "-fsanitize=address -fsanitize=undefined -rdynamic")
set(CMAKE_CXX_LINK_OPTIONS_RELEASE "")

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_MULTITHREADED OFF)

find_package(Catch2 2.13 REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(inc ${Boost_INCLUDE_DIR})

file(GLOB SOURCES "lib/*.cpp")
file(GLOB PATH_TRACER_SOURCES "app/*.cpp")
file(GLOB TEST_SOURCES "tests/*.cpp")

add_executable(path_tracer ${SOURCES} ${PATH_TRACER_SOURCES})
add_executable(tests ${SOURCES} ${TEST_SOURCES})

target_link_libraries(path_tracer Boost::program_options)
target_link_libraries(tests PRIVATE Catch2::Catch2 Boost::program_options)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()
