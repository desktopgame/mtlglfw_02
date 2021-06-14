cmake_minimum_required(VERSION 3.19)
project(mtlglfw_01 CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# 拡張機能のロード
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckSymbolExists)
# 各種変数の定義
if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

find_package(GLFW REQUIRED)

add_subdirectory(mtlpp)
add_executable(mtlglfw_01)
target_include_directories(mtlglfw_01 PUBLIC ${GLFW_INCLUDE_DIRS} mtlpp)
target_link_libraries(mtlglfw_01
        PRIVATE
        ${GLFW_LIBRARIES}
        mtlpp
)
target_compile_features(mtlglfw_01 PRIVATE cxx_std_17)
target_sources(mtlglfw_01 PRIVATE main.cpp)