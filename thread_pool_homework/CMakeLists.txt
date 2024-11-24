cmake_minimum_required(VERSION 3.16)
project(practice CXX)

# Build type
set(CMAKE_BUILD_TYPE "Debug" )
Set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
Set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(include)
add_subdirectory(src)

add_executable(main main.cpp)
add_executable(test test.cpp)
add_executable(test2 test2.cpp)
add_executable(test3 test3.cpp)
add_executable(test4 test4.cpp)
add_executable(test5 test5.cpp)

target_link_libraries(main PRIVATE SUNWEI pthread)
target_link_libraries(test PRIVATE pthread)
target_link_libraries(test2 PRIVATE pthread)
target_link_libraries(test4 PRIVATE pthread)
target_link_libraries(test5 PRIVATE pthread)