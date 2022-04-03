cmake_minimum_required (VERSION 3.16)
project (XMLConverter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Name Output File
set(TARGET_NAME xml_converter)

# Add Dependencies
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Set output as executable.
# TODO: This will eventually become a library when it gets integrated into burrito.
add_executable (${TARGET_NAME} ${SOURCES})

# Require C++ 17 or newer.
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

# Enable Extra Warnings and Errors
if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
