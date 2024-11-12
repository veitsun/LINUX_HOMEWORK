# select all cpp  in this directory
# 
file(GLOB HEllOLIB_SRC_cpp ${PROJECT_SOURCE_DIR}/thread_pool_homework/src/*.cpp)

# combime both list into hello_src

set(HELLOLIB_SRC $(HEllOLIB_SRC_cpp))

# create a  library from the collected source files
add_library(SUNWEI SHARED ${HELLOLIB_SRC})

