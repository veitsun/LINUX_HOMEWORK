# select all cpp  in this directory
# 
file(GLOB HELLOLIB_SRC_POOL ${PROJECT_SOURCE_DIR}/src/*.cpp)

# combime both list into hello_src

find_path(INCLUDE_DIR CThreadPool.h HINTS ${CMAKE_SOURCE_DIR}/include)
# 检查是否找到了路径
if(NOT INCLUDE_DIR)
    message(FATAL_ERROR "CThreadPool.h not found!")
endif()
include_directories(${INCLUDE_DIR})
# # 输出调试信息
# message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
# message(STATUS "Source files found: ${HELLOLIB_SRC_POOL}")

# create a  library from the collected source files
add_library(SUNWEI SHARED ${HELLOLIB_SRC_POOL})
target_include_directories(SUNWEI PUBLIC ${INCLUDE_DIR})


