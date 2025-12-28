# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.2)

project ("myMuduo")
#mymuduo最终编译成so动态库，设置动态库路径，放在根目录的lib文件夹下
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
#设置调试信息，以及启动C++11标准
set(CMAKE_CXX_FLAGES "${CMAKE_CXX_FLAGES} -g -std=C++11")
# Include sub-projects.
add_subdirectory ("myMuduo")
