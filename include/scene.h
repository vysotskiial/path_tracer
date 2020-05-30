#pragma once

#include <memory>
#include <string>
#include <vector>
#include "object.h"

enum Sides {
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back,
};

struct Camera {
	constexpr static double height_absolute = 1.1547;
	Ray camera;
	int width;
	int height;
	vec3 plane_x;
	vec3 plane_y;

	Camera(const Ray &cam, int w, int h);
};

class Scene {
private:
	constexpr static int max_depth = 8;
	std::vector<std::unique_ptr<Object>> objects;
	Intersection intersect(const Ray &r);
	Color get_color(const Ray &r, int depth);
	Camera camera;

public:
	Scene(const Camera &cam): camera(cam) {}
	void add_object(std::unique_ptr<Object> obj) { objects.push_back(move(obj)); }
	void add_cube(vec3 top_dir, const Triangle &bottom,
	              const std::array<Material, 6> &sides);
	void render_scene(std::string filename, int samples);
};
