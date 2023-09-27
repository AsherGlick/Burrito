cmake_minimum_required (VERSION 3.16)
project (XMLConverter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Generate Protobuf
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER proto/waypoint.proto)

# Name Output File
set(TARGET_NAME xml_converter)

# Add Dependencies
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Set output as executable.
# TODO: This will eventually become a library when it gets integrated into burrito.
add_executable (${TARGET_NAME} ${SOURCES} ${PROTO_SRC})

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

### TESTS ######################################################################
# Enable testing using CMake's built-in functionality
enable_testing()

set(TEST_TARGET_NAME xml_converter_tests)

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/xml_converter.cpp")

find_package(GTest REQUIRED)

add_executable(${TEST_TARGET_NAME} tests/test_string_hierarchy.cpp ${TEST_SOURCES} ${PROTO_SRC})

target_link_libraries(${TEST_TARGET_NAME} GTest::GTest GTest::Main)
target_link_libraries(${TEST_TARGET_NAME} ${Protobuf_LIBRARIES})

gtest_discover_tests(${TEST_TARGET_NAME})
