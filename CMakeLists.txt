cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(path_tracer LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS                "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_AR      "/usr/bin/llvm-ar")
set(CMAKE_LINKER  "/usr/bin/lld")
set(CMAKE_NM      "/usr/bin/llvm-nm")
set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
set(CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wshadow -Wconversion -lboost_program_options")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto=thin")

set(CMAKE_CXX_LINK_OPTIONS_DEBUG "-fsanitize=address -fsanitize=undefined -rdynamic")
set(CMAKE_CXX_LINK_OPTIONS_RELEASE "")


set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost)

include_directories(inc)
file(GLOB SOURCES "src/*.cpp")

add_executable(
    path_tracer
    ${SOURCES}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()
