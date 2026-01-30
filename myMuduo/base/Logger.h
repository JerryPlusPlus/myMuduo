#ifndef MYMUDUO_BASE_LOGGER_H
#define MYMUDUO_BASE_LOGGER_H
#include"myMuduo/base/LogStream.h"
#include"myMuduo/base/Timestamp.h"


namespace myMuduo {

	class TimeZone;

	class Logger {
	public:
		enum class LogLevel:char {	// 日志等级
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL,
			NUM_LOG_LEVELS,
		};
		/// <summary> 编译时期计算源文件的basename（不带目录的文件名） </summary>
		class SourceFile {
		public:
			template<int N> SourceFile(const char (&arr)[N])
				:data(arr),
				size_(N-1)
			{
				// 从一个文件路径字符串里找最后一个 /，用来提取“basename”（不带目录的文件名）
				const char* slash = strrchr(data_, '/');
				if (slash) {
					data_ = slash + 1;
					size_ -= static_cast<int>(data_ - arr);
				}
			}

			explicit SourceFile(const char* filename)
				:data_(filename) {
				const char* slash = strrchr(filename, '/');
				/*没有发生字符数组的拷贝，只是将指针指向filename的最后一个'/'的后一个字符的地址。
				需要注意的点这种写法的前提是：filename/arr 指向的字符串在 SourceFile 使用期间必须一直有效。
				•	__FILE__ 传进来时是静态存储期字符串，长期有效，没问题
				•	如果传的是临时 buffer（局部 char[]）并且 SourceFile 被保存到其生命周期之后，就会悬空指针*/
				if (slash)	data_ = slash + 1;	
				size_ = static_cast<int>(strlen(data_));
			}

			const char* data_;
			int size_;
		};

		Logger(SourceFile file, int line);
		Logger(SourceFile file, int line, LogLevel level);
		Logger(SourceFile file, int line, LogLevel level, const char* func);
		Logger(SourceFile file, int line, bool toAbort);
		~Logger();

		LogStream& stream() { return impl_.stream_; }

		static LogLevel logLevel();
		static void setLogLevel(LogLevel level);

		typedef void (*OutputFunc)(const char* msg, int len);
		typedef void (*FlushFunc)();
		static void setOutPut(OutputFunc);
		static void setFlush(FlushFunc);
		static void setTimeZone(const TimeZone& tz);

	private: 

		class Impl {
		public:
			typedef Logger::LogLevel LogLevel;
			Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
			void formatTime();
			void finish();

			Timestamp time_;
			LogStream stream_;
			LogLevel level_;
			int line_;
			SourceFile basename_;
		};

		Impl impl_;
	};

	//
	// CAUTION: do not write:
	//
	// if (good)
	//   LOG_INFO << "Good news";
	// else
	//   LOG_WARN << "Bad news";
	//
	// this expends to
	//
	// if (good)
	//   if (logging_INFO)
	//     logInfoStream << "Good news";
	//   else
	//     logWarnStream << "Bad news";
	//
	#define LOG_TRACE if(Logger::logLevel()<= Logger::LogLevel::TRACE)	\
		myMuduo::Logger(__FILE__,__LINE__,Logger::LogLevel::TRACE,__func__).stream()
	#define LOG_DEBUG if(Logger::logLevel()<= Logger::LogLevel::DEBUG)	\
		myMuduo::Logger(__FILE__,__LINE__,Logger::LogLevel::DEBUG,__func__).stream()
	#define LOG_INFO if(Logger::logLevel()<=Logger::LogLevel::INFO)		\
		myMuduo::Logger(__FILE__,__LINE__).stream()	
	#define LOG_WARN	myMuduo::Logger(__FILE__,__LINE__,Logger::LogLevel::WARN).stream()
	#define LOG_ERROR	myMuduo::Logger(__FILE__,__LINE__,Logger::LogLevel::ERROR).stream()
	#define LOG_FATAL	myMuduo::Logger(__FILE__,__LINE__,Logger::LogLevel::FATAL).stream()
	#define LOG_SYSERR	myMuduo::Logger(__FILE__,__LINE__,false).stream()
	#define LOG_SYSFATAL myMuduo::Logger(__FILE__,__LINE__,true).stream()

	const char* strerror_tl(int savedErrno);

	// Taken from glog/logging.h
	// Check that the input is non NULL. This very useful in constructor
	// initializer lists.

	#define CHECK_NOTNULL(val)	\
		::myMuduo::CheckNotNull(__FILE__,__LINE__,"'" #val "' Must be non NULL",(val))

	// A small helper for CHECK_NOTNULL().
	template<typename T>
	T* CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr) {
		if (!ptr)	myMuduo::Logger(file, line, Logger::LogLevel::FATAL).stream() << names;
		return ptr;
	}
	
}	// !namespace myMuduo

#endif // !MYMUDUO_BASE_LOGGER_H