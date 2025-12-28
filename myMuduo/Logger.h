#pragma once
#include<string>
#include<unordered_map>
#include"nocopyable.h"

//LOG_INFO(%s  %d, arg1, arg2)
#define LOG_INFO(logmsgFormat,...) \
do{ \
	Logger* logInstance = Logger::getInstance(); \
	logInstance->setLevel(LogLevel::INFO); \
	char buf[1024] = {0};\
	snprintf(buf,1024,logmsgFormat,##__VA__ARGS__); \
	logInstance->log(buf); \
}while(0)

#define LOG_ERROR(logmsgFormat,...) \
do{ \
	Logger* logInstance = Logger::getInstance(); \
	logInstance->setLevel(LogLevel::ERROR); \
	char buf[1024] = {0};\
	snprintf(buf,1024,logmsgFormat,##__VA__ARGS__); \
	logInstance->log(buf); \
}while(0)

#define LOG_FATAL(logmsgFormat,...) \
do{ \
	Logger* logInstance = Logger::getInstance(); \
	logInstance->setLevel(LogLevel::FATAL); \
	char buf[1024] = {0};\
	snprintf(buf,1024,logmsgFormat,##__VA__ARGS__); \
	logInstance->log(buf); \
}while(0)

#ifdef MUDEBUG

#define LOG_DEBUG(logmsgFormat,...) \
do{ \
	Logger* logInstance = Logger::getInstance(); \
	logInstance->setLevel(LogLevel::DEBUG); \
	char buf[1024] = {0};\
	snprintf(buf,1024,logmsgFormat,##__VA__ARGS__); \
	logInstance->log(buf); \
}while(0)

#else
	#define LOG_DEBUG(logmsgFormat,...)
#endif // DEBUG

#define LOGLEVEL_LIST(X) \
		X(INFO)	\
		X(ERROR) \
		X(FATAL) \
		X(DEBUG)

enum class LogLevel:ushort {
#define X(name) name, 
	LOGLEVEL_LIST(X)
#undef X
};

static std::unordered_map<LogLevel, std::string> map{
#define X(name) {LogLevel::name , #name},
	LOGLEVEL_LIST(X)
#undef X
};

class Logger : nocopyable {
public:
	static Logger* getInstance();
	LogLevel setLevel(LogLevel level);
	void log(const std::string msg);
private:
	Logger(LogLevel level = LogLevel::INFO);
	~Logger() noexcept;
	LogLevel level_;
	const std::unordered_map<LogLevel, std::string> map_{
		#define X(name) {LogLevel::name , #name},
			LOGLEVEL_LIST(X)
		#undef X
	};
};
