# select all cpp  in this directory
# 
file(GLOB HEllOLIB_SRC_BUFFER ${PROJECT_SOURCE_DIR}/src/*.cpp)

# combime both list into hello_src

find_path(INCLUDE_DIR bufferfile.h HINTS ${CMAKE_SOURCE_DIR}/include)

if(NOT INCLUDE_DIR)
    message(FATAL_ERROR "bufferfile.h not found !")
endif()

include_directories(${INCLUDE_DIR})


# create a  library from the collected source files
add_library(SUNWEI SHARED ${HEllOLIB_SRC_BUFFER})
target_include_directories(SUNWEI PUBLIC ${INCLUDE_DIR})

