cmake_minimum_required (VERSION 3.16)
project (XMLConverter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Name Output File
set(TARGET_NAME xml_converter)

# Add Protobuf
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
# include_directories()
EXECUTE_PROCESS(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --proto_path=generators/proto_templates --cpp_out=src/ generators/proto_templates/waypoint.proto) 

# Add Dependencies
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.cc")

# Set output as executable.
# TODO: This will eventually become a library when it gets integrated into burrito.
add_executable (${TARGET_NAME} ${SOURCES})

# include libraies
target_link_libraries(${TARGET_NAME} ${Protobuf_LIBRARIES})

# Require C++ 17 or newer.
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

# Enable Extra Warnings and Errors
if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
