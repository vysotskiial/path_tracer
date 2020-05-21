#include <iostream>
#include "math.h"

// Calculate determinant for matrix from its columns
double det(const vec3 &col1, const vec3 &col2, const vec3 &col3)
{
	return col1.x * col2.y * col3.z + col2.x * col3.y * col1.z +
	       col3.x * col1.y * col2.z - col3.x * col2.y * col1.z -
	       col2.x * col1.y * col3.z - col1.x * col3.y * col2.z;
}

vec3 vec3::get_orthogonal() const
{
	if (x == 0)
		return {1, 0, 0};
	if (y == 0)
		return {0, 1, 0};
	if (z == 0)
		return {0, 0, 1};
	double len = sqrt(z * z + y * y);
	return {0, -z / len, y / len};
}

vec3 operator+(const vec3 &lhs, const vec3 &rhs)
{
	vec3 res(lhs);
	res += rhs;
	return res;
}

vec3 operator-(const vec3 &lhs, const vec3 &rhs)
{
	vec3 res(lhs);
	res -= rhs;
	return res;
}

vec3 vector_prod(const vec3 &lhs, const vec3 &rhs)
{
	double new_x = lhs.y * rhs.z - lhs.z * rhs.y;
	double new_y = -lhs.x * rhs.z + lhs.z * rhs.x;
	double new_z = lhs.x * rhs.y - lhs.y * rhs.x;

	return {new_x, new_y, new_z};
}

double operator*(const vec3 &lhs, const vec3 &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

vec3 vec3::normalize()
{
	double len = sqrt(x * x + y * y + z * z);
	(*this) /= len;
	return *this;
}

void vec3::print()
{
	std::cout << x << " " << y << " " << z << std::endl;
}

vec3 &vec3::operator+=(const vec3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

vec3 vec3::operator-() const
{
	return vec3(-x, -y, -z);
}

vec3 &vec3::operator-=(const vec3 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
