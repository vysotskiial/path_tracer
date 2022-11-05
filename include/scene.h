#pragma once

#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <nlohmann/json.hpp>
#include <png++/image.hpp>
#include "object.h"

struct Emitter {
	Ray base; // Ray from point to the center of canvas
	int width;
	int height;
	double canvas_height = 1.1547; // canvas height / base length
	// Basis of canvas plane
	vec3 plane_x;
	vec3 plane_y;

	Emitter() = default;
	Emitter(const Ray &b, int w, int h, double abs);
};

struct ImageSegment {
	int x_start;
	int x_end;
	int y_start;
	int y_end;
};

class Scene {
private:
	Emitter emitter;
	std::mutex mut;
	int samples;
	png::image<png::rgb_pixel> image;
	std::vector<std::unique_ptr<Object>> objects;
	std::vector<ImageSegment> work_queue;

	Intersection intersect(const Ray &r);
	Color get_color(const Ray &r, int depth);
	void add_spheres(const nlohmann::json &j);
	template<Planar T>
	void add_planar_objects(const nlohmann::json &j, const std::string &name);
	void render_segment(ImageSegment s);
	void start_work();

public:
	Scene(const std::string &filename, int w, int h);
	void render_scene(const std::string &filename, int s, int threads);
};
