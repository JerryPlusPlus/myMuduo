#pragma once
#include"copyable.h"
#include<netinet/in.h>
#include<string>

// 封装socket地址类型
class InetAddress:public copyable {
public:
	explicit InetAddress(uint16_t port,std::string ip = "127.0.0.1");
	explicit InetAddress(const sockaddr_in& addr);
	std::string toIp() const;
	std::string toProt() const;
	std::string toIpPort() const;
	const sockaddr* getSockAddr()const;
private:
	union
	{	//使用联合体类型保存BSD Socket API中IPv4/IPv6套接字地址结构体的原始定义
		sockaddr_in addr_;
		sockaddr_in6 addr6_;
	};
};

//C:\Users\geniu\AppData\Local\Microsoft\Linux\HeaderCache\1.0\VSRemote\usr\include\strings.h