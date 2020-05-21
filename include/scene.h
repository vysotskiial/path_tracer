#pragma once

#include <memory>
#include <string>
#include <vector>
#include "object.h"

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
	std::vector<std::unique_ptr<Object>> objects;
	Ray intersect(const Ray &r);
	Camera camera;

public:
	Scene(const Camera &cam): camera(cam) {}
	void add_object(std::unique_ptr<Object> obj) { objects.push_back(move(obj)); }

	void render_scene(std::string filename, int samples_per_pixel);
};
