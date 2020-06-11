#pragma once
#include <algorithm>

//template<class T>
//inline void SafeDelete(T& pObjectToDelete)
//{
//	if (pObjectToDelete != nullptr)
//	{
//		delete(pObjectToDelete);
//		pObjectToDelete = nullptr;
//	}
//}

template<typename T>
inline void Clamp(T& value, T min, T max)
{
	if (value > max)
		value = max;

	if (value < min)
		value = min;
}