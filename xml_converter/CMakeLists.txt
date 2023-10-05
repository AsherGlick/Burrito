cmake_minimum_required (VERSION 3.16)
project (XMLConverter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


### Shared Infrastructure ######################################################
set(CORE_LIB core_lib)

# Generate Protobuf
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER proto/waypoint.proto)

# Add Dependencies
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/xml_converter.cpp")

# Create a static library for common sources
add_library(${CORE_LIB} STATIC ${SOURCES} ${PROTO_SRC})

# Include protobuf libraies.
target_link_libraries(${CORE_LIB} PUBLIC ${Protobuf_LIBRARIES})

# Require C++ 17 or newer.
target_compile_features(${CORE_LIB} PUBLIC cxx_std_17)
if(MSVC)
  target_compile_options(${CORE_LIB} PUBLIC /W4 /WX)
else()
  target_compile_options(${CORE_LIB} PUBLIC -Wall -Wextra -Wpedantic)
endif()


### CLI Executable #############################################################
# Name Output File
set(TARGET_NAME xml_converter)

# Set output as executable.
add_executable(${TARGET_NAME} src/xml_converter.cpp)
target_link_libraries(${TARGET_NAME} ${CORE_LIB})


### TESTS ######################################################################
set(TEST_TARGET_NAME test_xml_converter)

# Enable testing using CMake's built-in functionality to be able to run `make test`
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

find_package(GTest REQUIRED)

add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET_NAME} GTest::GTest GTest::Main)
target_link_libraries(${TEST_TARGET_NAME} ${CORE_LIB})

gtest_discover_tests(${TEST_TARGET_NAME})
