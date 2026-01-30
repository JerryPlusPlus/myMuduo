#ifndef MYMUDUO_BASE_COPYABLE_H
#define MYMUDUO_BASE_COPYABLE_H

namespace myMuduo {
	/// A tag class emphasises the objects are copyable.
	/// The empty base class optimization applies.
	/// Any derived class of copyable should be a value type.
	class copyable {
	protected: 
		copyable() = default;
		~copyable() = default;
	};
}	// !namespace myMuduo

#endif // !MYMUDUO_BASE_COPYABLE_H
