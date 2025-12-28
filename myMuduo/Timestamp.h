#pragma once
#include<iostream>

class Timestamp {
public:
	explicit Timestamp();
	explicit Timestamp(int64_t microSecondsSinceEpoch);
	~Timestamp() noexcept;
	static Timestamp now();
	std::string toString() const;
private:
	int64_t microSecondsSinceEpoch_;
};