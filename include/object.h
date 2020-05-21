#pragma once

#include "math.h"

struct Material {
	vec3 color;
	bool light{false};
	bool mirror{false};
	Material(vec3 col): color(col) {}
};

class Object {
protected:
	Material material;

public:
	// Return normal vector to object's surface in the point of intersection
	// second argument stands for time it takes ray to reach object
	// meaning ray.start + ray.dir * time is in object
	virtual Ray intersect(const Ray &ray, double &time) = 0;
	Object(const Material &m): material(m) {}
};

class Sphere : public Object {
private:
	vec3 center;
	double radius;

public:
	Sphere(const vec3 &c, double r, const Material &mat = vec3())
	  : Object(mat), center(c), radius(r)
	{
	}
	Ray intersect(const Ray &ray, double &time) final;
};

class Triangle : public Object {
private:
	vec3 A;
	vec3 B;
	vec3 C;

public:
	Triangle(const vec3 &_A, const vec3 &_B, const vec3 &_C,
	         const Material &mat = vec3())
	  : Object(mat), A(_A), B(_B), C(_C)
	{
	}
	Ray intersect(const Ray &ray, double &time) final;
};
