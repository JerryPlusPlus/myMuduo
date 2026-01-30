include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
	set_source_files_properties(SocketsOps.cpp PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()
#如果某个源文件没有被添加到CMakeLists.txt的编译源文件中，将会被作为orphan translation unit
#同时不会应用CmakeLists.txt文件中的包含路径，可能会导致头文件无法正确跳转/打开
set(net_SRCS
	#Acceptor.cpp 
	Buffer.cpp 
	#Channel.cpp 
	#Connector.cpp 
	#EventLoop.cpp 
	#EventLoopThread.cpp 
	#EventLoopThreadPool.cpp 
	InetAddress.cpp 
	#Poller.cpp 
	#poller/DefaultPoller.cpp 
	#poller/EPollPoller.cpp 
	#poller/PollPoller.cpp 
	Socket.cpp 
	SocketsOps.cpp 
	#TcpClient.cpp 
	#TcpConnection.cpp 
	#TcpServer.cpp 
	#Timer.cpp 
	#TimerQueue.cpp
)

add_library(myMuduo_net ${net_SRCS})
target_link_libraries(myMuduo_net muduo_base)

#add_library(muduo_net_cpp11 ${net_SRCS})
#target_link_libraries(muduo_net_cpp11 muduo_base_cpp11)
#set_target_properties(muduo_net_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS myMuduo_net DESTINATION lib)
#install(TARGET muduo_net_cpp11 DESTINATION lib)

set(HEADERS
	#Buffer.h 
	#Callbacks.h 
	#Channel.h 
	Endian.h 
	#EventLoop.h 
	#EventLoopThread.h 
	#EventLoopThreadPool.h 
	InetAddress.h 
	#TcpClient.h 
	#TcpConnection.h 
	#TcpServer.h 
	TimerId.h 
)
install(FILES ${HEADERS} DESTINATION "include/myMuduo/net")

add_subdirectory(http)
add_subdirectory(inspect)

if(MYMUDUO_BUILD_EXAMPLES)
	add_subdirectory(tests)
endif()

if(PROTOBUF_FOUND)
	add_subdirectory(protobuf)
	add_subdirectory(protorpc)
else()
	add_subdirectory(protobuf EXCLUDE_FROM_ALL)
	add_subdirectory(protorpc EXCLUDE_FROM_ALL)
endif()
