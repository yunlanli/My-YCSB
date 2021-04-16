cmake_minimum_required(VERSION 3.10)
project(My_YCSB)

set(CMAKE_CXX_STANDARD 14)

add_executable(run run.cpp client.cpp client.h workload.cpp workload.h measurement.cpp measurement.h)
target_link_libraries(run /usr/local/lib/libwiredtiger.so pthread)
