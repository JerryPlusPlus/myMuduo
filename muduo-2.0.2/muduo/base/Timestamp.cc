// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/base/Timestamp.h"

#include <sys/time.h>
#include <stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

using namespace muduo;

static_assert(sizeof(Timestamp) == sizeof(int64_t),
              "Timestamp is same size as int64_t");

string Timestamp::toString() const	//将一个“自 Unix Epoch（1970-01-01 00:00:00 UTC）以来的微秒时间戳”转换为字符串表示
{
  //{0}保证buf数组内的元素全部初始化为\0，int64_t 取值范围为-(2^63+1)~2^63,-(2^63+1)=-9,223,372,036,854,775,808。
  //最坏情况下buf要使用28字节(符号1字节+秒数19字节+小数点1字节+微秒6字节+\0)
  //32 字节是“面向类型极限、跨平台、安全、可维护性的工程选择”。muduo 不是在“省字节”，而是在“省未来的 bug”。
  
  char buf[32] = {0};	
  //整数除法得到自Epoch起的秒数。其中microSeconds表示总微秒数，kMircroSecondsPerSecond表示ms/s，是常量，一般为1000000
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;	
  int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;	//取余得到当前秒内的微秒偏移
  // 格式化输出。PRId64是 C/C++ 标准为 int64_t 提供的跨平台printf格式说明符宏。在Linux x86_64上： #define PRId64 "ld"。在Windows MSVC上：#define PRId64 "lld" 
  snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);	
  return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds) const
{
  char buf[64] = {0};
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
  struct tm tm_time;
  gmtime_r(&seconds, &tm_time);

  if (showMicroseconds)
  {
    int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
  }
  else
  {
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}

Timestamp Timestamp::now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);	//调用了系统api函数，获取时间信息
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

