cmake_minimum_required(VERSION 3.15)
project(UsagiWorkspace VERSION 0.1 LANGUAGES CXX)

if(WIN32)
    add_definitions(-DUSAGI_PLATFORM_WINDOWS)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定使用的是g++编译器
# set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_BUILD_TYPE Debug)

# 构建类型，如果未指定，默认为 Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR})

# 添加子项目
add_subdirectory(Usagi)
add_subdirectory(Sandbox)
