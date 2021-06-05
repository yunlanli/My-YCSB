cmake_minimum_required(VERSION 3.10)
project(My_YCSB)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
include_directories(core/include)

set(CoreSource core/client.cpp
               core/measurement.cpp
               core/worker.cpp
               core/workload.cpp)

set(WiredTigerClientSource wiredtiger/wt_client.cpp)
add_executable(init_wt ${CoreSource} ${WiredTigerClientSource} wiredtiger/init_wt.cpp)
add_executable(run_wt_uniform ${CoreSource} ${WiredTigerClientSource} wiredtiger/run_wt_uniform.cpp)
add_executable(run_wt_zipfian ${CoreSource} ${WiredTigerClientSource} wiredtiger/run_wt_zipfian.cpp)
add_executable(run_wt_zipfian_rmw ${CoreSource} ${WiredTigerClientSource} wiredtiger/run_wt_zipfian_rmw.cpp)
add_executable(run_wt_latest ${CoreSource} ${WiredTigerClientSource} wiredtiger/run_wt_latest.cpp)
target_link_libraries(init_wt /usr/local/lib/libwiredtiger.so pthread)
target_link_libraries(run_wt_uniform /usr/local/lib/libwiredtiger.so pthread)
target_link_libraries(run_wt_zipfian /usr/local/lib/libwiredtiger.so pthread)
target_link_libraries(run_wt_zipfian_rmw /usr/local/lib/libwiredtiger.so pthread)
target_link_libraries(run_wt_latest /usr/local/lib/libwiredtiger.so pthread)

set(RedisClientSource redis/redis_client.cpp)
add_executable(init_redis ${CoreSource} ${RedisClientSource} redis/init_redis.cpp)
add_executable(run_redis ${CoreSource} ${RedisClientSource} redis/run_redis.cpp)
target_link_libraries(init_redis pthread hiredis)
target_link_libraries(run_redis pthread hiredis)
