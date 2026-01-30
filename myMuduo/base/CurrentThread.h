#ifndef MYMUDUO_BASE_CURRENTTHREAD_H
#define MYMUDUO_BASE_CURRENTTHREAD_H

#include"myMuduo/base/Types.h"

namespace myMuduo {
	namespace CurrentThread {
		extern __thread int t_cacheTid;
		extern __thread char t_tidString[32];
		extern __thread int t_tidStringLength;
		extern __thread const char* t_threadName;
		void cacheTid();
		
		inline int tid() {
			if(__builtin_expect(0==t_cacheTid,0)) cacheTid();
			return t_cacheTid;
		}
		inline const char* tidString() { return t_tidString; }	// for logging
		inline int tidStringLength() { return t_tidStringLength; }	// for logging
		inline const char* name() { return t_threadName; }
		bool isMainThread();
		void sleepUsec(int64_t usec);	// for testing

		string stackTrace(bool demangle);

	}	// !namespace CurrentThread
}	// !namespace CurrentThread

#endif // ！MYMUDUO_BASE_CURRENTTHREAD_H