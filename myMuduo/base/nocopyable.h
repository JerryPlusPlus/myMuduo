#ifndef MYMUDUO_BASE_NOCOPYABLE
#define MYMUDUO_BASE_NOCOPYABLE

namespace myMuduo {

	/*
	nocopyable类可以被继承，但不能被复制或赋值。
	*/

	class nocopyable {
	public:
		template<typename T>
		nocopyable(T&&) = delete;
		template<typename T>
		void operator=(T&&) = delete;
	protected:
		nocopyable() = default;
		~nocopyable() = default;
	};
}	// !namespace myMuduo

#endif // !MYMUDUO_BASE_NOCOPYABLE


