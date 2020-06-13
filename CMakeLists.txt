cmake_minimum_required (VERSION 2.8)
project (test) 

 # 设置c++编译选项
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -g -std=c++11 ")

# 添加预处理器定义
add_definitions(-DDEBUG) 

# 输出信息
message(STATUS "CXX_FLAGS=${CMAKE_CXX_FLAGS}")

# 添加TutorialConfig.h的路径到头文件的搜索路径
include_directories("${PROJECT_BINARY_DIR}")
#include_directories ("${PROJECT_SOURCE_DIR}/topk")

# 保证库被编译
#add_subdirectory (MathFunctions)
 
 set(EXTRA_LIBS gtest)

# 添加源文件
aux_source_directory(. DIR_SOURCES)
aux_source_directory(./topk DIR_SOURCES)

# 库
#target_link_libraries (test ${EXTRA_LIBS})
link_directories(/usr/lib64/)
link_libraries(${EXTRA_LIBS})

# 添加目标可执行文件
add_executable (test ${DIR_SOURCES})
