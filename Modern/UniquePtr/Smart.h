#pragma once
#include <memory>
#include <iostream>

template<typename T>
class Smart
{
public:
	using SharedPtr = typename std::shared_ptr<T>;

	static std::unique_ptr<T, void(*)(T*)> New()
	{
		// 람다를 넣어서 할 수 있는 건가?
		std::unique_ptr<T, void(*)(T*)> ptr(new T, [](T* obj)
			{
				printf("unique_ptr 의 해제도 일어난다!!!\n");
				obj->Reset();
				delete obj;
			});

		return ptr;
	}
};