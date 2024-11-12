cmake_minimum_required(VERSION 3.16)
project(practice CXX)

# Build type
set(CMAKE_BUILD_TYPE "Debug" )
Set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
Set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(./include)
add_subdirectory(./src)

add_executable(sunwei sunwei.cpp)
add_executable(spp spp.cpp)
add_executable(sunwei2 sunwei2.cpp)


target_link_libraries(sunwei PRIVATE SUNWEI pthread)
target_link_libraries(spp PRIVATE SUNWEI pthread)
target_link_libraries(sunwei2 PRIVATE SUNWEI pthread)