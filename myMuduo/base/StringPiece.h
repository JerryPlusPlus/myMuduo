#ifndef MYMUDUO_BASE_STRINGPIECE_H
#define MYMUDUO_BASE_STRINGPIECE_H

#include<string.h>
#include<iosfwd>	// for ostream forward-declaration

#include"myMuduo/base/Types.h"

namespace myMuduo {

	// For passing C-style string argument to a function.
	class StringArg { //copyable
	private:
		const char* str_;
	public:
		StringArg(const char* str)
			:str_(str)
		{
		}
		StringArg(const string& str)
			:str_(str.c_str())
		{
		}
		const char* c_str() const { return str_; }

	};

	class StringPiece {
	public:
		StringPiece()
			: ptr_(nullptr),length_(0)
		{ }
		StringPiece(const char* str)
			:ptr_(str),length_(static_cast<int>(strlen(ptr_)))
		{ }
		StringPiece(const unsigned char* str)
			:ptr_(reinterpret_cast<const char*>(str)),length_(static_cast<int>(strlen(ptr_)))
		{ }	// 我接收的是一段只读的原始字节数据（byte buffer），并假定它是一个以 \0 结尾的字符串。
		StringPiece(const string& str)
			:ptr_(str.c_str()),length_(static_cast<int>(str.size()))
		{ }
		StringPiece(const char* offset,int len)
			:ptr_(offset),length_(len)
		{ }
		
		const char* data() const { return ptr_; }
		int size()const { return length_; }
		bool empty()const { return 0 == length_; }
		const char* begin()const { return ptr_; }
		const char* end()const { return ptr_ + length_; }

		void clear() { ptr_ = nullptr; length_ = 0; }
		void set(const char* buffer, int len) { ptr_ = buffer; length_ = len; }
		void set(const char* str) { ptr_ = str; length_ = static_cast<int>(strlen(str)); }
		void set(const void* buffer, int len) { ptr_ = reinterpret_cast<const char*>(buffer); length_ = len; }

		char operator[](int i)const { return ptr_[i]; }
		void remove_prefix(int n) { ptr_ += n; length_ -= n; }
		void remove_suffix(int n) { length_ -= n; }

		bool operator==(const StringPiece& x)const 
		{ return x.length_ == length_ && strcmp(x.ptr_, ptr_) == 0; }
		bool operator!=(const StringPiece& x)const { return !(*this == x); }
		//定义宏函数。 memcmp 用于逐字节比较两块内存的内容,n 必须是实际可访问的字节数，否则会导致 未定义行为
#define STRINGPIECE_BINARY_PREDICATE(cmp,auxcmp)									\
		bool operator cmp(const StringPiece& x )const {								\
			int r = memcmp(ptr_,x.ptr_,length_< x.length_? length_:x.length_);		\
			return (r auxcmp 0)||((0 == r && ( length_ == x.length_ )));				\
		}													
		STRINGPIECE_BINARY_PREDICATE(< , < );	// 宏调用 
		STRINGPIECE_BINARY_PREDICATE(<= , < );	// 宏调用 
		STRINGPIECE_BINARY_PREDICATE(> , > );	// 宏调用 
		STRINGPIECE_BINARY_PREDICATE(>= , > );	// 宏调用 
#undef STRINGPIECE_BINARY_PREDICATE
		int compare(const StringPiece& x)const {
			int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
			if (0 == r) {
				if (length_ < x.length_) return -1;
				else return +1;
			}
			return r;
		}

		string as_string()const {
			return string(data(), size());
		}

		void CopyToString(string* target)const {
			target->assign(ptr_, length_);
		}

		//判断x是否是*this的子串
		bool starts_with(const StringPiece& x)const {
			return (length_ >= x.length_) && (0 == memcmp(ptr_, x.ptr_, x.length_));
		}
	private:
		const char* ptr_;
		int			length_;
	};	// StringPiece

}	// namespace myMuduo

#ifdef HAVE_TYPE_TRAITS 
// this makes vector<StringPiece> really fast for some STL implementations
template<> struct  __type_traits<myMuduo::StringPiece>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
#endif

//允许StringPiece被日志正常输出(未定义，muduo-2.0.2中没有找到具体定义的位置，所以定义暂时没写)
std::ostream& operator<<(std::ostream& os, const myMuduo::StringPiece& piece);

#endif // !MYMUDUO_BASE_STRINGPIECE_H