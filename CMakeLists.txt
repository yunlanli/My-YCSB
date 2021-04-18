cmake_minimum_required(VERSION 3.10)
project(My_YCSB)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
include_directories(core/include)

set(CoreSource core/client.cpp
               core/measurement.cpp
               core/worker.cpp
               core/workload.cpp)

set(WiredTigerSource wiredtiger/wt_client.cpp wiredtiger/run_wt.cpp)

add_executable(run_wt ${CoreSource} ${WiredTigerSource})
target_link_libraries(run_wt /usr/local/lib/libwiredtiger.so pthread)

