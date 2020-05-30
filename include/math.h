#pragma once
#include <cmath>

struct vec3 {
	double x{0};
	double y{0};
	double z{0};

	vec3() = default;
	vec3(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
	vec3(const vec3 &other) = default;

	void print() const;
	double length_sqr() const { return x * x + y * y + z * z; }
	double length() const { return sqrt(x * x + y * y + z * z); }
	bool operator==(const vec3 &other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	vec3 &operator+=(const vec3 &other);
	vec3 operator-() const;
	vec3 &operator-=(const vec3 &other);
	vec3 normalize();
	vec3 get_orthogonal() const;

	vec3 &operator*=(double d)
	{
		x *= d;
		y *= d;
		z *= d;
		return *this;
	}

	vec3 &operator/=(double d)
	{
		x /= d;
		y /= d;
		z /= d;
		return *this;
	}

	friend vec3 operator*(double d, const vec3 &vec)
	{
		vec3 res(vec);
		res *= d;
		return res;
	}

	friend vec3 operator*(const vec3 &vec, double d) { return d * vec; }

	friend vec3 operator/(const vec3 &vec, double d)
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
inline vec3 mult(const vec3 &lhs, const vec3 &rhs)
{
	vec3 res{lhs};
	res.x *= rhs.x;
	res.y *= rhs.y;
	res.z *= rhs.z;
	return res;
}
