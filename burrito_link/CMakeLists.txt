cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

project (burrito_link)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# remove `-rdynamic` default link library flag set in the linux `Platform/*.cmake` file
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

# Set the compiler to mingw so we can build a windows executable on linux
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)

# Create the standalone executable
set(STANDALONE burrito_link.exe)
add_executable (${STANDALONE} "burrito_link.c")
target_link_libraries(${STANDALONE} PRIVATE "ws2_32")

target_compile_options(${STANDALONE} PRIVATE -Wall -Os -g)

# An option specifically for creating console application and not a windowed application
target_compile_options(${STANDALONE} PRIVATE -mconsole)
