cmake_minimum_required(VERSION 3.19)
project(mtlglfw_02 CXX)

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
find_package(Eigen REQUIRED)

add_subdirectory(mtlpp)
add_executable(mtlglfw_02)
target_include_directories(mtlglfw_02 PUBLIC ${GLFW_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS} mtlpp)
target_link_libraries(mtlglfw_02
        PRIVATE
        ${GLFW_LIBRARIES}
        mtlpp
)
target_compile_features(mtlglfw_02 PRIVATE cxx_std_17)
target_sources(mtlglfw_02 PRIVATE main.cpp)