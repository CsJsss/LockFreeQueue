cmake_minimum_required(VERSION 3.16)
project(LockFreeQueue)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS -pthread)

#add_executable(LockFreeQueue  LockFreeQueue.h check.cpp setting.h)
add_executable(LockFreeQueue  LockFreeQueue.h  setting.h benchmark_lockfree.cpp)
#add_executable(LockFreeQueue  LockFreeQueue.h  setting.h benchmark_mutex.cpp)
