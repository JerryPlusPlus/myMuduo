#ifndef MYMUDUO_BASE_TIMEZONE_H
#define MYMUDUO_BASE_TIMEZONE_H

#include"myMuduo/base/copyable.h"
#include<memory>
#include<time.h>

namespace myMuduo {

	// TimeZone for 1970-2030
	class TimeZone :public myMuduo::copyable {
	public:
		explicit TimeZone(const char* zonfile);
		TimeZone(int eastOfUtc, const char* tzname);	// a fixed timezone
		TimeZone() = default;	 // an invalid timezone

		// default copy ctor/assigment/dtor are Okay. ctor => constructor | dtor => destructor

		bool valid() const { return static_cast<bool>(data_); }
		tm toLocalTime(time_t secondsSinceEpoch)const;
		time_t fromLocalTime(const struct tm&)const;

		// gmtime(3)
		static tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
		// timegm(3)
		static time_t fromUtcTime(const struct tm&);
		//year in [1900..2500],month in [1..12],day in [1..31]
		static time_t fromUtcTime(int year, int month, int day, int hour, int mintue, int seconds);
		
		struct Data;

	private:
		std::shared_ptr<Data> data_;
	};

}	// !namespace myMuduo


#endif // !MYMUDUO_BASE_TIMEZONE_H
