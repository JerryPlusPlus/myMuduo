#pragma once
/*
nocopyable类可以被继承，但不能被复制或赋值。
*/

class nocopyable {
public:
	template<typename T>
	nocopyable(T&&) = delete;
	template<typename T>
	auto operator=(T&&) = delete;
protected:
	nocopyable() = default;
	~nocopyable() = default;
};