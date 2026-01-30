#ifndef	MYMUDUO_BASE_DATE_H
#define MYMUDUO_BASE_DATE_H

#include"myMuduo/base/copyable.h"
#include"myMuduo/base/Types.h"

struct tm;

namespace myMuduo {

	class Date : public myMuduo::copyable {
	public:

		struct YearMonthDay
		{
			int year;	// [1900..2500]
			int month;	// [1..12]
			int day;	// [1..31]
		};
		
		static const int kDaysPerWeek = 7;
		static const int kJulianDayOf1970_01_01;

		///<summary> 构造一个无效的Date </summary>
		Date():julianDayNumber_(0){ }

		///<sumary> constructs a yyyy-mm-dd Date
		///<parma parma="month> 1<= month <= 12 </parma>
		Date(int year, int month, int day);

		/// <summary>
		/// Construct a Date from Julian Day Number.
		/// </summary>
		explicit Date(int julianDayNum):julianDayNumber_(julianDayNum){ }

		/// <summary>
		///	Construct a Date from struct tm
		/// </summary>
		explicit Date(struct tm& tm_time);

		// default copy/assignment/dtor are Okay. dtor => destructor的缩写

		void swap(Date& that) {
			std::swap(julianDayNumber_, that.julianDayNumber_);
		}

		bool valid()const { return julianDayNumber_ > 0; }

		///<summary> Convert to yyyy-mm-dd format.
		string toIsoString() const;

		YearMonthDay yearMonthDay() const;

		int year() const { return yearMonthDay().year; }
		int month()const { return yearMonthDay().month; }
		int day() const { return yearMonthDay().day; }

		// [0,1,...,6] => [Sunday, Monday, ... , Saturday]

		int weekDay() const { return (julianDayNumber_ + 1) % kDaysPerWeek; }
		int julianDayNumber()const { return kDaysPerWeek; }


	private:
		/*Julian Day Number（JDN，儒略日数） 是一种在天文学与时间计算中广泛使用的 连续日计数体系，
		用于将“日期”映射为一个 单调递增的整数（或实数），从而避免历法转换、闰年规则等带来的复杂性。*/
		int julianDayNumber_;
	};

	inline bool operator<(Date x, Date y) {
		return x.julianDayNumber() < y.julianDayNumber();
	}

}	// !namespace myMuduo


#endif	// !MYMUDUO_BASE_DATE_H
