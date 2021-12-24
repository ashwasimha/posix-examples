project(posix_mq)

cmake_minimum_required(VERSION 2.8.12)

#enable c++11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find threads to link against the binary
find_package (Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)


add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
#TARGET_LINK_LIBRARIES( average-calculator
#                       pthread
#                       rt
#                     )