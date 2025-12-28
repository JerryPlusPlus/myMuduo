# CMakeList.txt : CMake project for myMuduo, include source and define
# project specific logic here.
#

# Add source to this project's executable.

#cmake自动搜索当前目录下的所有.cpp和.h文件，放到MYMUDUO_SOURCES变量中
file(GLOB MYMUDUO_SOURCES 
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
add_library(myMuduo SHARED ${MYMUDUO_SOURCES})

target_include_directories(myMuduo PRIVATE /usr/local/include)

# TODO: Add tests and install targets if needed.