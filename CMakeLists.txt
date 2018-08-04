cmake_minimum_required(VERSION 3.5)

project(mysql_wrapper)

include_directories(/usr/include/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -std=c++17 -pthread")

file(GLOB src_all 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(mysql_wrapper STATIC ${src_all})
target_link_libraries(mysql_wrapper ${OPENSSL_LIBRARIES} mysqlclient)

