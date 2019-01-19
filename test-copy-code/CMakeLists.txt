cmake_minimum_required(VERSION 2.8)

project(test-copy)

set (CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} test-copy.cpp)
target_link_libraries( ${PROJECT_NAME} )
