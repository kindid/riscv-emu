cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

project(rv32tst)

add_executable(rv32tst ${SOURCES} ${HEADERS})
