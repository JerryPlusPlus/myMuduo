set(base_SRCS
	#AsyncLogging.cpp
	#Condition.cpp
	#CountDownLatch.cpp
	CurrentThread.cpp
	Date.cpp
	#Exception.cpp
	#FileUtil.cpp
	#LogFile.cpp
	Logger.cpp
	LogStream.cpp
	#ProcessInfo.cpp
	Timestamp.cpp
	#Thread.cpp
	#ThreadPool.cpp
	TimeZone.cpp
)

add_library(myMuduo_base ${base_SRCS} )
target_link_libraries(myMuduo_base pthread rt)

#add_library(muduo_base_cpp11 ${base_SRCS})
#target_link_libraries(muduo_base_cpp11 pthread rt)
#set_target_properties(muduo_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS myMuduo_base DESTINATION lib)
#install(TARGETS muduo_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/myMuduo/base)

if(MYMUDUO_BUILD_EXAMPLES)
	add_subdirectory(tests)
endif()
