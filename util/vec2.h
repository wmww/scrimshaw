#pragma once

#include <string>
#include <cmath>

template<typename T>
struct Vec2
{
	T x, y;

	Vec2() : x{}, y{} {}
	Vec2(T x, T y) : x{x}, y{y} {}

	template<typename U>
	inline operator Vec2<U>() const
	{
		return Vec2<U>(x, y);
	}

	inline bool operator==(const Vec2<T>& other) const { return x == other.x && y == other.y; }
	inline bool operator!=(const Vec2<T>& other) const { return x != other.x || y != other.y; }
	inline Vec2<T> operator+(const Vec2<T>& other) const { return Vec2<T>(x + other.x, y + other.y); }
	inline void operator+=(const Vec2<T>& other)
	{
		x += other.x;
		y += other.y;
	}
	inline Vec2<T> operator-(const Vec2<T>& other) const { return Vec2<T>(x - other.x, y - other.y); }
	inline Vec2<T> operator-() const { return Vec2<T>(-x, -y); }
	inline void operator-=(const Vec2<T>& other)
	{
		x -= other.x;
		y -= other.y;
	}
	inline Vec2<T> operator*(T value) const { return Vec2<T>(x * value, y * value); }
	inline void operator*=(T value)
	{
		x *= value;
		y *= value;
	}
	inline Vec2<T> operator/(T value) const { return Vec2<T>(x / value, y / value); }
	inline void operator/=(T value)
	{
		x /= value;
		y /= value;
	}

	inline T size() const { return x * y; }
	inline T length_squared() const { return x * x + y * y; }
	inline T length() const { return sqrt(x * x + y * y); }

	std::string to_string() const { return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ")"; }
};

typedef Vec2<int> Vec2i;
typedef Vec2<double> Vec2d;
