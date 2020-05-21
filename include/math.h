#pragma once
#include <cmath>

struct vec3 {
	double x{0};
	double y{0};
	double z{0};

	vec3() = default;
	vec3(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
	vec3(const vec3 &other) = default;

	operator bool() const { return x || y || z; }
	void print();

	vec3 &operator+=(const vec3 &other);
	vec3 operator-() const;
	vec3 &operator-=(const vec3 &other);
	vec3 normalize();
	vec3 get_orthogonal() const;

	template<typename Num>
	vec3 &operator*=(Num d)
	{
		x *= d;
		y *= d;
		z *= d;
		return *this;
	}

	template<typename Num>
	vec3 &operator/=(Num d)
	{
		x /= d;
		y /= d;
		z /= d;
		return *this;
	}

	template<typename Num>
	friend vec3 operator*(Num d, const vec3 &vec)
	{
		vec3 res(vec);
		res *= d;
		return res;
	}

	template<typename Num>
	friend vec3 operator*(const vec3 &vec, Num d)
	{
		return d * vec;
	}

	template<typename Num>
	friend vec3 operator/(const vec3 &vec, Num d)
	{
		vec3 res(vec);
		res /= d;
		return res;
	}
};

vec3 vector_prod(const vec3 &lhs, const vec3 &rhs);
double operator*(const vec3 &lhs, const vec3 &rhs);
double det(const vec3 &col1, const vec3 &col2, const vec3 &col3);
vec3 operator+(const vec3 &lhs, const vec3 &rhs);
vec3 operator-(const vec3 &lhs, const vec3 &rhs);

struct Ray {
	vec3 start;
	vec3 dir;
	vec3 color;
};
