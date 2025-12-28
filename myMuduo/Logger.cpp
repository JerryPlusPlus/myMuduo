#pragma once

#include"Logger.h"
#include<iostream>

Logger* Logger::getInstance() {
	static Logger* log = new Logger();
	return log;
}

LogLevel Logger::setLevel(LogLevel level) {
	level_ = level;
	return level_;
}
//日志格式： [级别信息] time : msg
void Logger::log(const std::string msg) {
	std::cout << map[level_];
	//"写完timestamp类后在补全"
	std::cout << "print time" << " : " << msg << std::endl;
}

Logger::Logger(LogLevel level):level_(level) { }

Logger::~Logger() noexcept{ }
