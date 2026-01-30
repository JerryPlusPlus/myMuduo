#include"myMuduo/net/InetAddress.h"

#include"myMuduo/base/Logger.h"
#include"myMuduo/net/Endian.h"
#include"myMuduo/net/SocketsOps.h"

#include<netdb.h>
#include<netinet/in.h>
#include"stddef.h"

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;
#pragma GCC diagnostic error "-Wold-style-cast"

//     /* Structure describing an Internet socket address.  */
//		/*对 BSD Socket API 中 IPv4 / IPv6 套接字地址结构体的原始定义说明。*/
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };
//注意：这是“人为整理出来的展示代码”，不是编译器真正看到的代码顺序
//以 Linux glibc 的 <netinet/in.h> 为例，真实情况是：struct in_addr 在前面已经被声明过
//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

//     struct sockaddr_in6 {
//         sa_family_t     sin6_family;   /* address family: AF_INET6 */
//         uint16_t        sin6_port;     /* port in network byte order */
//         uint32_t        sin6_flowinfo; /* IPv6 flow information */
//         struct in6_addr sin6_addr;     /* IPv6 address */
//         uint32_t        sin6_scope_id; /* IPv6 scope-id */
//     };

using namespace myMuduo;
using namespace myMuduo::net;

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6),
	"InetAddress is same size as sockaddr_in6");
static_assert(0 == offsetof(struct sockaddr_in, sin_family), "sin_family offset 0");
static_assert(0 == offsetof(struct sockaddr_in6, sin6_family), "sin6_family offset 0");
static_assert(2 == offsetof(struct sockaddr_in, sin_port), "sin_port offset 2");
static_assert(2 == offsetof(sockaddr_in6, sin6_port), "sin6_port offset 2");

InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6) {
	static_assert(0 == offsetof(InetAddress, addr6_), "addr6_ offset 0");
	static_assert(0 == offsetof(InetAddress, addr_), "addr_ offset 0");
	if (ipv6) {
		memZero(&addr6_ ,sizeof addr6_);
		addr6_.sin6_family = AF_INET6;
		/*
		•	IPv4：常以 uint32_t 整数形式处理，需要 htonl/hostToNetwork32
		•	IPv6：以 16 bytes 地址结构处理，直接拷贝即可；字节序已经是协议定义的顺序，不做“端序转换”这一层
			唯一仍然要转换的是 端口：sin6_port 是 16-bit 整数，所以你做了 hostToNetwork16(port)，这是必须的。
		*/
		in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
		addr6_.sin6_addr = ip;
		addr6_.sin6_port = sockets::hostToNetwork16(port);
	}
	else {
		memZero(&addr_, sizeof addr_);
		addr_.sin_family = AF_INET;
		in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
		addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
		addr_.sin_port = sockets::hostToNetwork16(port);
	}
}

InetAddress::InetAddress(StringArg ip, uint16_t port, bool ipv6) {
	if (ipv6) {
		memZero(&addr6_, sizeof addr6_);
		sockets::fromIpPort(ip.c_str(), port, &addr6_);
	}
	else {
		memZero(&addr_, sizeof addr_);
		sockets::fromIpPort(ip.c_str(), port, &addr_);
	}
}

string InetAddress::toIpPort()const {
	char buf[64] = "";
	sockets::toIpPort(buf, sizeof buf, getSockAddr());
	return buf;
}
string InetAddress::toIp()const {
	char buf[64] = "";
	sockets::toIp(buf, sizeof buf, getSockAddr());
	return buf;
}
uint32_t InetAddress::ipNetEndian()const {
	assert(family() == AF_INET);
	return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::toPort()const { 
	return sockets::networkToHost16(portNetEndian()); 
}

static __thread char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(StringArg hostname, InetAddress* out) {
	assert(out != nullptr);
	struct hostent hent;
	struct hostent* he = nullptr;
	int herrno = 0;
	memZero(&hent, sizeof(hent));
	int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer,
		sizeof t_resolveBuffer, &he, &herrno);
	if (0 == ret && nullptr == he) {
		assert(AF_INET == he->h_addrtype && sizeof(uint32_t) == he->h_length);
		out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
		return true;
	}
	else {
		if (ret)	LOG_SYSERR << "InetAddress::reslove";
		return false;
	}
}

void InetAddress::setScopeId(uint32_t scope_id) {
	if (family() == AF_INET6)	addr6_.sin6_scope_id = scope_id;
}

