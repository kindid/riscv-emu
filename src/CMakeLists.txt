cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

project(tst)

include(GoogleTest)

#include_directories(thirdparty/wise_enum-master)

set(GTEST ${PROJECT_SOURCE_DIR}/googletest)

add_subdirectory(${GTEST})
include_directories(${GTEST}/googletest/include)
#target_link_libraries(tst ${GTEST}/build/libs/libgtest)

#include_directories(../googletest)
#include_directories(../googletest/include)
#add_subdirectory(../googletest)

add_executable(tst ${SOURCES} ${HEADERS})

target_link_directories(tst PUBLIC ${GTEST}/build/lib)

#add_executable(test_bitops bitops.cpp test_bitops.cpp)
target_link_libraries(tst gtest)
#target_link_libraries(tst ${GTEST}/build/libs/libgtest)

# function/macro - there's a *really* simple use case here
add_executable(bitops_tst bitops.cpp bitops_tst.cpp bitops.h)
include_directories(bitops_tst ${GTEST}/googletest/include)
target_link_directories(bitops_tst PUBLIC ${GTEST}/build/lib)
target_link_libraries(bitops_tst gtest)
target_link_libraries(bitops_tst gtest_main)

#target_link_libraries(test_bitops gtest gtest_main)
#include_directories(test_bitops ${GTEST}/googletest/googletest/include)
#target_link_libraries(test_bitops ${GTEST}/build/lib/libgtest)
#set(GTEST ${PROJECT_SOURCE_DIR}/thirdparty/googletest)
#set(GTEST ../googletest)


