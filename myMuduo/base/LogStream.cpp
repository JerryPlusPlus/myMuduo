#include"myMuduo/base/LogStream.h"
#include<algorithm>
#include<limits>
#include<type_traits>
#include<string.h>
#include<stdint.h>
#include<stdio.h>

#ifndef NDBUG
#include<assert.h>
#endif // !NDBUG


#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif // !__STDC_FORMAT_MACROS

#include<inttypes.h>

using namespace myMuduo;
using namespace myMuduo::detail;

// TODO: better itoa.
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wtautological-compare"
#else
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

namespace myMuduo {
	namespace detail {
		const char digits[] = "9876543210123456789";	// 在“负数取模”场景下，彻底消除分支、abs() 和溢出风险。
		const char* zero = digits + 9;
		static_assert(20 == sizeof(digits), "worng number of digits");

		const char digitsHex[] = "0123456789ABCDEF";//十六进制输出关注的是二进制位的值，与整数数学意义无关，不需要考虑负数。
		
		// 高效地将数字转换为数字字符串
		template<typename T>
		size_t convert(char buf[], T value) {	//转换为十进制字符串
			T i = value;
			char* p = buf;
			do
			{
				int lsd = static_cast<int>(i % 10);	//lsd（Least Significant Digit）
				i /= 10;
				*p++ = zero[lsd];
			} while (i!=0);
			if (value < 0) *p++ = '-';
			*p = '\0';
			std::reverse(buf, p);	// reverse elements in [_First, _Last)

			return p - buf;
		}

		size_t convertHex(char buf[], uintptr_t value) {	//转换为十六进制字符串
			uintptr_t i = value;
			char* p = buf;
			do
			{
				int lsd = static_cast<int>(i % 16);
				i /= 16;
				*p++ = digitsHex[lsd];
			} while (i!=0);
			*p = '\0';
			std::reverse(buf, p);

			return p - buf;
		}
		template class FixedBuffer<kSmallBuffer>;
		template class FixedBuffer<kLargerBuffer>;
		
	}	// !namespace detail
	/*
	 Format a number with 5 characters, including SI units.
	 [0,     999]
	 [1.00k, 999k]
	 [1.00M, 999M]
	 [1.00G, 999G]
	 [1.00T, 999T]
	 [1.00P, 999P]
	 [1.00E, inf)
	*/
	std::string formatSI(int64_t s) {
		double n = static_cast<double>(s);
		char buf[64];
		if (s < 1000)
			snprintf(buf, sizeof(buf), "%" PRId64, s);
		else if (s < 9995)
			snprintf(buf, sizeof(buf), "%.2fk", n / 1e3);
		else if (s < 99950)
			snprintf(buf, sizeof(buf), "%.1fk", n / 1e3);
		else if (s < 999500)
			snprintf(buf, sizeof(buf), "%.0fk", n / 1e3);
		else if (s < 9995000)
			snprintf(buf, sizeof(buf), "%.2fM", n / 1e6);
		else if (s < 99950000)
			snprintf(buf, sizeof(buf), "%.1fM", n / 1e6);
		else if (s < 999500000)
			snprintf(buf, sizeof(buf), "%.0fM", n / 1e6);
		else if (s < 9995000000)
			snprintf(buf, sizeof(buf), "%.2fG", n / 1e9);
		else if (s < 99950000000)
			snprintf(buf, sizeof(buf), "%.1fG", n / 1e9);
		else if (s < 999500000000)
			snprintf(buf, sizeof(buf), "%.0fG", n / 1e9);
		else if (s < 9995000000000)
			snprintf(buf, sizeof(buf), "%.2fT", n / 1e12);
		else if (s < 99950000000000)
			snprintf(buf, sizeof(buf), "%.1fT", n / 1e12);
		else if (s < 999500000000000)
			snprintf(buf, sizeof(buf), "%.0fT", n / 1e12);
		else if (s < 9995000000000000)
			snprintf(buf, sizeof(buf), "%.2fP", n / 1e15);
		else if (s < 99950000000000000)
			snprintf(buf, sizeof(buf), "%.1fP", n / 1e15);
		else if (s < 999500000000000000)
			snprintf(buf, sizeof(buf), "%.0fP", n / 1e15);
		else
			snprintf(buf, sizeof(buf), "%.2fE", n / 1e18);
		return buf;
	}
	/*
	 [0, 1023]
	 [1.00Ki, 9.99Ki]
	 [10.0Ki, 99.9Ki]
	 [ 100Ki, 1023Ki]
	 [1.00Mi, 9.99Mi]
	*/
	std::string fromatIEC(int64_t s) {
		double n = static_cast<double>(s);
		char buf[64];
		const double Ki = 1024.0;
		const double Mi = Ki * 1024.0;
		const double Gi = Mi * 1024.0;
		const double Ti = Gi * 1024.0;
		const double Pi = Ti * 1024.0;
		const double Ei = Pi * 1024.0;

		if (n < Ki)
			snprintf(buf, sizeof buf, "%" PRId64, s);   //跨平台的兼容写法：#define PRId64 "lld"
		else if (n < Ki * 9.995)
			snprintf(buf, sizeof buf, "%.2fKi", n / Ki);
		else if (n < Ki * 99.95)
			snprintf(buf, sizeof buf, "%.1fKi", n / Ki);
		else if (n < Ki * 1023.5)
			snprintf(buf, sizeof buf, "%.0fKi", n / Ki);

		else if (n < Mi * 9.995)
			snprintf(buf, sizeof buf, "%.2fMi", n / Mi);
		else if (n < Mi * 99.95)
			snprintf(buf, sizeof buf, "%.1fMi", n / Mi);
		else if (n < Mi * 1023.5)
			snprintf(buf, sizeof buf, "%.0fMi", n / Mi);

		else if (n < Gi * 9.995)
			snprintf(buf, sizeof buf, "%.2fGi", n / Gi);
		else if (n < Gi * 99.95)
			snprintf(buf, sizeof buf, "%.1fGi", n / Gi);
		else if (n < Gi * 1023.5)
			snprintf(buf, sizeof buf, "%.0fGi", n / Gi);

		else if (n < Ti * 9.995)
			snprintf(buf, sizeof buf, "%.2fTi", n / Ti);
		else if (n < Ti * 99.95)
			snprintf(buf, sizeof buf, "%.1fTi", n / Ti);
		else if (n < Ti * 1023.5)
			snprintf(buf, sizeof buf, "%.0fTi", n / Ti);

		else if (n < Pi * 9.995)
			snprintf(buf, sizeof buf, "%.2fPi", n / Pi);
		else if (n < Pi * 99.95)
			snprintf(buf, sizeof buf, "%.1fPi", n / Pi);
		else if (n < Pi * 1023.5)
			snprintf(buf, sizeof buf, "%.0fPi", n / Pi);

		else if (n < Ei * 9.995)
			snprintf(buf, sizeof buf, "%.2fEi", n / Ei);
		else
			snprintf(buf, sizeof buf, "%.1fEi", n / Ei);
		return buf;
	}
	

}	// !namespace myMuduo

// 由于前面使用了using namespace myMuduo; 和 using namespace mymyMuduo::detail;
// 所以下面类中的函数定义不能再写到命名空间中了

template<int SIZE>
const char* FixedBuffer<SIZE>::debugString() {
	*cur_ = '\0';
	return data_;
}

template<int SIZE> void FixedBuffer<SIZE>::cookieStart(){ }

template<int SIZE> void FixedBuffer<SIZE>::cookieEnd(){ }

void LogStream::staticCheck() {	// static const int kMaxNumericSize = 32;
	//std::numeric_limits<T>::digits10   类型 T 在十进制下，能保证无损表示的最大有效数字位数
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
		"KMaxNumericSize is large enought");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
		"kMaxNumericSize is large enought");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10,
		"kMaxNumericSize is large enought");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10,
		"kMaxNumericSize is large enought");
}

template<typename T>
void LogStream::formatInteger(T v) {
	if (buffer_.avail() > = kMaxNumericSize) {
		size_t  len = convert(buffer_.current(), v);
		buffer_.add(len);
	}
}

#define LOGSTREAM_OPERATOR_OSTREAM(Type,T)	\
LogStream & LogStream::operator<<(Type T){	\
	formatInteger(T);						\
	return *this;							\
}
LOGSTREAM_OPERATOR_OSTREAM(short,v);
LOGSTREAM_OPERATOR_OSTREAM(unsigned short, v);
LOGSTREAM_OPERATOR_OSTREAM(int, v);
LOGSTREAM_OPERATOR_OSTREAM(unsigned int, v);
LOGSTREAM_OPERATOR_OSTREAM(long,v);
LOGSTREAM_OPERATOR_OSTREAM(unsigned long , v);
LOGSTREAM_OPERATOR_OSTREAM(long long ,v);
LOGSTREAM_OPERATOR_OSTREAM(unsigned long long ,v);

#undef LOGSTREAM_OPERATOR_OSTREAM
/*
#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
	#ifdef _WIN64
		typedef unsigned __int64  uintptr_t;
	#else
		typedef unsigned int uintptr_t;
	#endif
#endif
*/
//uintptr_t 的本质不是“某种整数类型”，而是“一个语义承诺：它一定能完整跨平台地表示一个指针”。
LogStream& LogStream::operator<<(const void* p) {
	uintptr_t v = reinterpret_cast<uintptr_t>(p);	// uintptr_t v 保存的是指针 p 的地址数值表示
	if (buffer_.avail() >= kMaxNumericSize) {
		char* buf = buffer_.current();
		buf[0] = '0';
		buf[1] = 'x';
		size_t len = convertHex(buf + 2, v);	// 将地址数值转换为十六进制的字符串表示
		buffer_.add(len + 2);
	}
	return *this;
}

// FIXME:replace this with Grisu3 by Florian Loitsch.
LogStream& LogStream::operator<<(double v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
		buffer_.add(len);
	}
	return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt, T val) {
	static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic  type");

	length_ = snprintf(buf_, sizeof buf_, fmt, val);
	asser(static_cast<size_t>(length_) < sizeof buf_);
}

// 显式实例化 template<typename T> Fmt(const char* fmt, T val);

template Fmt::Fmt(const char* fmt, char);
template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);
template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);

