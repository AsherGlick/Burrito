cmake_minimum_required(VERSION 3.21)
project (XMLConverter)

# Set CPP standard library to cpp17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


################################################################################
# Core Library
#
# A core library that is shared between all of the export types. CLI executable,
# linked library, and test suite.
################################################################################
set(CORE_LIB core_lib)

# Generate Protobuf
set(Protobuf_USE_STATIC_LIBS ON)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER proto/guildpoint.proto)

# Add Dependencies
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/burrito_converter.cpp")

# Create a static library for common sources
add_library(${CORE_LIB} STATIC ${SOURCES} ${PROTO_SRC})

# Include protobuf libraies.
target_link_libraries(${CORE_LIB} PUBLIC ${Protobuf_LIBRARIES})


if(MSVC)
  target_compile_options(${CORE_LIB} PUBLIC /W4 /WX)
else()
  target_compile_options(${CORE_LIB} PUBLIC -Wall -Wextra -Wpedantic -Werror=return-type)
endif()


################################################################################
# CLI Executable
################################################################################
# Name Output File
set(TARGET_NAME burrito_converter)

# Set output as executable.
add_executable(${TARGET_NAME} src/burrito_converter.cpp)
target_link_libraries(${TARGET_NAME} ${CORE_LIB})


################################################################################
# Unit Tests Executable
################################################################################
set(TEST_TARGET_NAME test_burrito_converter)

# Enable testing using CMake's built-in functionality to be able to run `make test`
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

find_package(GTest REQUIRED)

add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET_NAME} GTest::GTest GTest::Main)
target_link_libraries(${TEST_TARGET_NAME} ${CORE_LIB})

gtest_discover_tests(${TEST_TARGET_NAME})
