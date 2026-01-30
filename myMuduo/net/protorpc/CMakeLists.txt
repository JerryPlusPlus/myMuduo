add_custom_command(OUTPUT rpc.pb.cc rpc.pb.h
	COMMAND protoc
	ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/rpc.proto -I${CMAKE_CURRENT_SOURCE_DIR}
	DEPENDS rpc.proto
	VERBATIM )
set_source_files_properties(rpc.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion")
include_directories(${PROJECT_BINARY_DIR})

add_library(myMuduo_protorpc_wire PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
set_target_properties(myMuduo_protorpc_wire PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")

#add_library(myMuduo_protorpc_wire_cpp11 rpc.pc.cc RpcCodec.cc)
#set_target_properties(myMuduo_protorpc_wire_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x -Wno-error=shadow")

if(MYMUDUO_BUILD_EXAMPLES)
	add_executable(protobuf_rpc_wire_test RpcCodec_test.cc)
	target_link_libraries(protobuf_rpc_wire_test myMuduo_protorpc_wire myMuduo_protobuf_codec)
	set_target_properties(protobuf_rpc_wire_test PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
endif()

add_library(myMuduo_protorpc RpcChannel.cc RpcServer.cc)
set_target_properties(myMuduo_protorpc PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(myMuduo_protorpc myMuduo_protorpc_wire myMuduo_protobuf_codec myMuduo_net protobuf z)

if(TCMALLOC_LIBRARY)
	target_link_libraries(myMuduo_protorpc tcmalloc_and_profiler)
endif()

install(TARGETS myMuduo_protorpc_wire myMuduo_protorpc DESTINATION lib)
#install(TARGETS myMuduo_protorpc_wire_cpp11 DESTINATION lib)

set(HEADERS 
	RpcCodec.h 
	RpcChannel.h 
	RpcServer.h 
	rpc.proto 
	rpcservice.proto 
	"${PROJECT_BINARY_DIR}/myMuduo/net/protorpc/rpc.pb.h"
)
install(FILES ${HEADERS} DESTINATION "include/myMuduo/net/protorpc")


