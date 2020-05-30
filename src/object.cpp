#include <iostream>
#include <cmath>
#include "object.h"

using namespace std;

constexpr double eps = 0.0001;

Intersection Sphere::intersect(const Ray &ray)
{
	Intersection res;
	vec3 ray_to_center = center - ray.start;

	// Quadratic equation for intersection a*t^2 + b*t + c = 0
	// where:
	double a = ray.dir * ray.dir;
	double b = ray.dir * ray_to_center * -2;
	double c = ray_to_center * ray_to_center - radius * radius;
	// Normalize coeffs
	b /= a;
	c /= a;
	a = 1;

	double discrim = b * b - 4 * c;

	if (discrim < 0) {
		return res;
	}
	double sqrt_disc = sqrt(discrim);

	res.time = (-b - sqrt_disc) / 2;
	if (res.time < -sqrt_disc + eps) {
		res.time = 0;
		return res;
	}

	if (res.time < eps)
		res.time += sqrt_disc;
	res.ray.start = ray.start + res.time * ray.dir;
	res.ray.dir = (res.ray.start - center) / radius;
	res.material = material;

	return res;
}

Intersection Triangle::intersect(const Ray &ray)
{
	Intersection res;
	vec3 e1 = B - A;
	vec3 e2 = C - A;
	vec3 e = ray.start - A;
	double denom = -det(e1, e2, ray.dir);

	if (fabs(denom) < 0.001)
		return res;

	double u = -det(e, e2, ray.dir) / denom;
	if (u > 1. || u < 0.)
		return res;
	double v = -det(e1, e, ray.dir) / denom;
	if (v < 0. || u + v > 1.)
		return res;
	res.time = det(e1, e2, e) / denom;
	if (res.time < eps) {
		res.time = 0.;
		return res;
	}

	res.ray.start = ray.start + res.time * ray.dir;
	res.ray.dir = vector_prod(e1, e2);
	if (res.ray.dir * ray.dir > 0)
		res.ray.dir = -res.ray.dir;
	res.ray.dir.normalize();
	res.material = material;
	return res;
}
