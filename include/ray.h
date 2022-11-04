#pragma once

#include "math.h"

using Color = vec3;

struct Material {
	Color color;
	bool light{false};
	bool mirror{false};
	Material(Color col): color(col) {}
	Material() = default;
};

struct Ray {
	vec3 start;
	vec3 dir;
};

struct Intersection {
	Ray ray{};
	double time{std::numeric_limits<double>().max()};
	Material material{};
};
