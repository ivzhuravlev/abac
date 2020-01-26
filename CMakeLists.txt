cmake_minimum_required(VERSION 3.0.0)

project(abac)

option(RUN_TESTS OFF)

if(RUN_TESTS)
add_definitions(-DRUN_TESTS)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(H_FILES src/ast.h
			src/token.h
			src/catch.hpp)
set(CPP_FILES src/main.cpp
			src/ast.cpp
			src/token.cpp
			src/ast_tests.cpp
			src/token_tests.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_executable(${CMAKE_PROJECT_NAME} ${H_FILES} ${CPP_FILES})

set_property(TARGET abac PROPERTY VS_STARTUP_PROJECT abac)