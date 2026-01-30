#ifndef MYMUDUO_NET_ENDIAN_H
#define MYMUDUO_NET_ENDIAN_H
#include<stdint.h>
#include<endian.h>

namespace myMuduo {
	namespace net {
		namespace sockets {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

//把主机字节序（host byte order）的 32 位无符号整数，转换为网络字节序（network byte order）
//TCP/IP协议族硬性规定： 网络字节序 = 大端序(big-endian)
/*
htobeX的命名规范：
	h  -> host(主机字节序)
	to -> 转换
	be -> big-endian(大端序)
	X  -> X位整数
例如：htobe32中的32表示为32位整数
*/
			inline uint64_t hostToNetwork64(uint64_t host64) { return htobe64(host64); }
			inline uint32_t hostToNetwork32(uint32_t host32) { return htobe32(host32); }
			inline uint16_t hostToNetwork16(uint16_t host16) { return htobe16(host16); }
			
			// 将网络字节序转化为主机使用的字节序
			inline uint64_t networkToHost64(uint64_t net64) { return be64toh(net64); }
			inline uint32_t networkToHost32(uint32_t net32) { return be32toh(net32); }
			inline uint16_t networkToHost16(uint16_t net16) { return be16toh(net16); }

#pragma GCC diagnostic pop

		}	// !namespace sockets
	}	// !namespace net
}	// !namespace myMuduo

#endif // !MYMUDUO_NET_ENDIAN_H
