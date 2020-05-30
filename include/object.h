#pragma once

#include "ray.h"

class Object {
protected:
	Material material;

public:
	// Return normal vector to object's surface in the point of intersection
	// second argument stands for time it takes ray to reach object
	// meaning ray.start + ray.dir * time is in object
	virtual Intersection intersect(const Ray &ray) = 0;
	Object(const Material &m): material(m) {}
	Material mat() const { return material; }
};

class PlaneObject : public Object {
protected:
	// Plane is defined by three points
	// TODO check that they're not laying on one line
	vec3 A;
	vec3 B;
	vec3 C;

	virtual bool check_intersection(std::pair<double, double> coords) = 0;

public:
	PlaneObject(const vec3 &_A, const vec3 &_B, const vec3 &_C,
	            const Material &mat = vec3())
	  : Object(mat), A(_A), B(_B), C(_C)
	{
	}

	vec3 getA() const { return A; }
	vec3 getB() const { return B; }
	vec3 getC() const { return C; }

	Intersection intersect(const Ray &ray) final;
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
	Intersection intersect(const Ray &ray) final;
};

class Triangle : public PlaneObject {
private:
	bool check_intersection(std::pair<double, double> coords) final
	{
		return (coords.first >= 0. && coords.second >= 0 &&
		        coords.first + coords.second <= 1.);
	}

public:
	Triangle(const vec3 &_A, const vec3 &_B, const vec3 &_C,
	         const Material &mat = vec3())
	  : PlaneObject(_A, _B, _C, mat)
	{
	}
};

class Rectangle : public PlaneObject {
private:
	bool check_intersection(std::pair<double, double> coords) final
	{
		return (coords.first >= 0. && coords.second >= 0. && coords.first <= 1. &&
		        coords.second <= 1.);
	}

public:
	Rectangle(const vec3 &_A, const vec3 &_B, const vec3 &_C,
	          const Material &mat = vec3())
	  : PlaneObject(_A, _B, _C, mat)
	{
	}
};

class Circle : public PlaneObject {
private:
	double rad_sqr;

	bool check_intersection(std::pair<double, double> coords) final
	{
		vec3 to_center = (B - A) * coords.first + (C - A) * coords.second;
		return (to_center.length_sqr() <= rad_sqr);
	}

public:
	Circle(const vec3 &center, const vec3 &_B, const vec3 &_C,
	       const Material &mat = vec3())
	  : PlaneObject(center, _B, _C, mat), rad_sqr((B - A).length_sqr())
	{
	}
};
