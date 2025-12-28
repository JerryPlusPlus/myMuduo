#include"InetAddress.h"
#include<strings.h>

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6),
	"InetAddress is same size as sockaddr_in6");


InetAddress::InetAddress(uint16_t port, std::string ip) {
	bzero(&addr_, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_zero;

	addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}
InetAddress::InetAddress(const sockaddr_in& addr){
}
std::string InetAddress::toIp() const{ 
}
std::string InetAddress::toProt() const{
}
std::string InetAddress::toIpPort() const{
}
const sockaddr* InetAddress::getSockAddr()const{
}



