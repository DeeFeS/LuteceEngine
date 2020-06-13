#pragma once
#include <algorithm>
#include <glm/vec2.hpp>

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

inline float Dot(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

inline float MagnitudeSq(const glm::vec2& v)
{
	return Dot(v, v);
}

inline float Magnitude(const glm::vec2& v)
{
	return sqrtf(MagnitudeSq(v));
}

inline bool InBounds(const glm::vec2& p, const glm::vec2& tl, const glm::vec2& br)
{
	return tl.x < p.x&& p.x < br.x&& tl.y < p.y&& p.y < br.y;
}

inline bool InBounds(const glm::vec2& p, const glm::vec2& center, const float radius)
{
	return MagnitudeSq(p - center) < powf(radius, 2.f);
}