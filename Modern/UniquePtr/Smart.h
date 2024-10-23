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
		// ���ٸ� �־ �� �� �ִ� �ǰ�?
		std::unique_ptr<T, void(*)(T*)> ptr(new T, [](T* obj)
			{
				printf("unique_ptr �� ������ �Ͼ��!!!\n");
				obj->Reset();
				delete obj;
			});

		return ptr;
	}
};