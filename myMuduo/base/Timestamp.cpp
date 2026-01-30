#include"myMuduo/base/Timestamp.h"
#include<sys/time.h>
#include<time.h>	// C标准库头文件
#include<stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif 

#include<inttypes.h>

using namespace myMuduo;

static_assert(sizeof(Timestamp) == sizeof(int64_t),
	"Timestamp is same size of int64_t");

/// <summary>
/// 将一个“自Unix Epoch（1970-01-01 00:00:00 UTC）以来的微秒时间戳”转换为字符串（seconds.microseconds）表示。
/// </summary>
/// <returns>返回字符串（seconds.microseconds）</returns>
string Timestamp::toString()const {
	char buf[32]{ 0 };
	int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
	int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
	return buf;
}

///	<summary>
/// 将一个“自Uinx Epoch（1970:01:01 00:00:00 UTC）以来的微秒时间戳”转换为字符串(年/月/日 时:分:秒.微秒)表示
/// </summary>
/// <param name="showMicroseconds> 布尔值，是否显示microseconds </parma>
string Timestamp::toFormattedString(bool showMicrosenconds)const {
	char buf[64]{ 0 };
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);

	if (showMicrosenconds)
		snprintf(buf, sizeof buf, "%4d/%02d/%02d %02d:%02d:%02d.%06d",
			tm_time.tm_year + 1900,
			tm_time.tm_mon + 1,
			tm_time.tm_mday,
			tm_time.tm_hour,
			tm_time.tm_min,
			tm_time.tm_sec,
			microSecondsSinceEpoch() % kMicroSecondsPerSecond
		);
	else 
		snprintf(buf, sizeof buf, "%4d/%02d/%02d %02d:%02d:%02d",
			tm_time.tm_year + 1900,
			tm_time.tm_mon + 1,
			tm_time.tm_mday,
			tm_time.tm_hour,
			tm_time.tm_min,
			tm_time.tm_sec
		);
		return buf;
}

/// <summary>
/// 返回一个Timestamp表示相对于Unix Epoch的时间戳
/// </summary>
Timestamp Timestamp::now() {
	struct timeval tv;
	//调用系统api函数，获取时间信息
	if (0 != gettimeofday(&tv, nullptr)) {	// gettimeofday是在sys/time.h头文件中被声明
		return Timestamp::invalid();
	}
	const int64_t seconds = static_cast<int64_t>(tv.tv_sec);
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

