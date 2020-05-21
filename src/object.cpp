#include <iostream>
#include <cmath>
#include "object.h"

using namespace std;

Ray Sphere::intersect(const Ray &ray, double &t)
{
	Ray res;
	t = 0;
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

	if (discrim < 0)
		return res;
	double sqrt_disc = sqrt(discrim);

	t = (-b - sqrt_disc) / 2;
	if (t > 0)
		return {ray.start + t * ray.dir, {0, 0, 1}, material.color}; // TODO random
		                                                             // direction
	t += sqrt_disc;
	if (t > 0)
		return {ray.start + t * ray.dir, {0, 0, 1}, material.color}; // TODO random
		                                                             // direction

	t = 0;
	return res;
}

Ray Triangle::intersect(const Ray &ray, double &time)
{
	Ray res;
	time = 0;
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
	time = det(e1, e2, e) / denom;
	if (time < 0)
		time = 0;

	res.start = A + u * e1 + v * e2;
	res.color = material.color;
	return res;
}
