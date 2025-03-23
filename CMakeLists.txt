# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(YourProjectName)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件搜索路径
include_directories(
    ${CMAKE_SOURCE_DIR}/classes
    ${CMAKE_SOURCE_DIR}/util
)

# 查找所有的实现文件
file(GLOB_RECURSE IMPL_SOURCES "${CMAKE_SOURCE_DIR}/impl/*.cc")
file(GLOB_RECURSE UTIL_SOURCES "${CMAKE_SOURCE_DIR}/util/*.cc")

# 添加可执行文件
add_executable(your_executable
    ${IMPL_SOURCES}
    ${UTIL_SOURCES}
    ${CMAKE_SOURCE_DIR}/main.cc
)