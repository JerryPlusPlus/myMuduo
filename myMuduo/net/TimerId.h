#ifndef MYMUDUO_NET_TIMERID_H
#define MYMUDUO_NET_TIMERID_H
#include"myMuduo/base/copyable.h"
#include"myMuduo/base/Types.h"

// This is a public header file, it must only include public header files.

namespace myMuduo {
	namespace net {
		class Timer;

		///
		/// an opaque identifier, for canceling Timre.
		/// 
		class TimerId : public myMuduo::copyable {
		public:
			TimerId() :timer_(nullptr),sequence_(0) { }
			TimerId(Timer* timer,int64_t seq):timer_(timer),sequence_(seq){}

			// default copy-ctor, dcor and assignment are okay

			friend class TimerQueue;

		private:
			Timer* timer_;
			int64_t sequence_;
		};

	}	// !namespace net
}	// !namespace myMuduo

#endif // !MYMUDUO_NET_TIMERID_H