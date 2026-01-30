#ifndef MYMUDUO_BASE_TYPES_H
#define MYMUDUO_BASE_TYPES_H

#include<stdint.h>	// 这里虽然没有直接使用这些类型，但这是“基础类型头”的统一入口之一。
#include<string.h>
#include<string>

#include<assert.h>


///
/// 👉 Debug 时抓 bug，Release 时零开销
///

namespace myMuduo {
	using std::string;	//本头文件对外暴露 myMuduo::string 别名，方便在整个 Muduo 代码中统一使用 string

	inline void memZero(void* p, size_t n) {	//显式语义的 memset
		memset(p, 0, n);
	}
	//可选地 强制编译器检查：From → To 是否是“合法的隐式转换”
	template<typename To,typename From>	
	inline To implicit_cast(From const& f) {
		return f;
	}


	template<typename To,typename From>
	inline To down_cast(From* f) {
		if (false) implicit_cast<From*, To>(nullptr);	// To --> From* 的隐式转换是否合法
#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
		assert(nullptr == f || nullptr != dynamiv_cast<To>(f));		//RTTI: From* --> To的隐式转换是否合法
#endif 
		return static_cast<To>(f);	// 向下转换时f指向的对象真的是*To类型的对象，可以将f安全的转换为*To类型
	}

}

#endif // !MYMUDUO_BASE_TYPES_H
