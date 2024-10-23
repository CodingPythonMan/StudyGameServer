#pragma once
#include "SmartPool.h"

//#define USE_MEMORY_POOL

template<typename T>
class Smart
{
public:
#ifdef USE_MEMORY_POOL
	using UniquePtr = typename SmartPool<T>::AutoReleaseUniquePtr;
#endif
	using SharedPtr = typename std::shared_ptr<T>;

	virtual ~Smart() = default;
	// 요건 순수 가상함수로 반드시 재정의하게 만든다.
	virtual void Reset() = 0;

#ifdef USE_MEMORY_POOL
	
#else

#endif

};

