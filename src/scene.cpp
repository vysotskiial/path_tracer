#include <algorithm>
#include <cmath>
#include <random>
#include <png++/image.hpp>
#include <png++/png.hpp>
#include <numeric>
#include "scene.h"

using namespace std;
using namespace nlohmann;

Emitter::Emitter(const Ray &b, int w, int h, double abs)
  : base(b), width(w), height(h), canvas_height(abs)
{
	plane_x = base.dir.get_orthogonal();
	plane_y = vector_prod(base.dir, plane_x).normalize();
	double normal_coeff = canvas_height * base.dir.length() / height;
	plane_y = plane_y * normal_coeff;
	plane_x = plane_x * normal_coeff;
}

static inline vec3 json2v(const json &j)
{
	return {j.at(0), j.at(1), j.at(2)};
}

static inline Material getMaterial(const json &j)
{
	Material m;
	if (j.count("color"))
		m = json2v(j.at("color"));
	if (j.count("mirror"))
		m.mirror = j.at("mirror");
	else if (j.count("light"))
		m.light = j.at("light");
	return m;
}

template<Planar T>
void Scene::add_planar_objects(const json &j, const std::string &name)
{
	if (!j.count(name))
		return;
	for (auto &obj : j.at(name)) {
		auto O = json2v(obj.at("zero"));
		auto e1 = json2v(obj.at("basis").at(0));
		auto e2 = json2v(obj.at("basis").at(1));

		objects.push_back(make_unique<T>(O, e1, e2, getMaterial(obj)));
	}
}

void Scene::add_spheres(const json &j)
{
	if (!j.count("spheres"))
		return;
	for (auto &obj : j.at("spheres")) {
		auto O = json2v(obj.at("center"));

		objects.push_back(
		  make_unique<Sphere>(O, obj.at("radius"), getMaterial(obj)));
	}
}

Scene::Scene(const string &filename, int width, int height)
{
	ifstream ifs(filename);
	auto config = json::parse(ifs, nullptr, true, true);
	auto cam = config.at("camera");
	auto cam_start = json2v(cam.at("start"));
	auto cam_dir = json2v(cam.at("direction"));
	emitter = {{cam_start, cam_dir}, width, height, cam.at("canvas_height")};
	add_planar_objects<Triangle>(config, "triangles");
	add_planar_objects<Rectangle>(config, "rectangles");
	add_planar_objects<Circle>(config, "circles");
	add_spheres(config);
	for (auto &obj : objects) {
		cerr << obj->mat().mirror << '\n';
	}
}

Intersection Scene::intersect(const Ray &r)
{
	Intersection result{};
	for (auto &obj : objects) {
		auto i = obj->intersect(r);
		if (i.time < result.time)
			result = i;
	}

	return result;
}

mt19937::result_type seed = 0;
mt19937 gen(seed);
uniform_real_distribution<double> distr(0., 1.);
inline double ddrand()
{
	return 2 * drand48() - 1.;
}

Color Scene::get_color(const Ray &r, int depth)
{
	Intersection i = intersect(r);

	Color c = i.material.color;
	if (++depth > 5) {
		auto max_color = max({c.x, c.y, c.z});
		if (drand48() < max_color)
			c /= max_color;
		else
			return {};
	}

	// Didn't hit anything
	if (i.time == std::numeric_limits<double>().max()) {
		return {};
	}

	if (i.material.light)
		return {12., 12., 12.};
	if (i.material.mirror) {
		vec3 new_dir = r.dir - 2 * (i.ray.dir * r.dir) * i.ray.dir;
		return get_color(Ray{{i.ray.start}, {new_dir}}, depth);
	}

	double r1 = drand48() * M_PI * 2;
	double r2 = drand48();
	double r2s = sqrt(r2);
	vec3 u = i.ray.dir.get_orthogonal();
	vec3 v = vector_prod(i.ray.dir, u);
	i.ray.dir = sqrt(1 - r2) * i.ray.dir + cos(r1) * r2s * u + sin(r1) * r2s * v;
	return mult(c, get_color(i.ray, depth));
}

// gamma correction
inline int toInt(double x)
{
	return int(pow(std::clamp(x, 0., 1.), 1 / 2.2) * 255 + .5);
}

void Scene::render_scene(const string &filename, int samples)
{
	png::image<png::rgb_pixel> image(emitter.width, emitter.height);
	Ray r;
	r.start = emitter.base.start;
	for (int i = -emitter.height / 2; i < emitter.height / 2; i++) {
		for (int j = -emitter.width / 2; j < emitter.width / 2; j++) {
			r.dir = emitter.base.dir + emitter.plane_x * j + emitter.plane_y * i;
			Color color;
			for (auto k = 0; k < samples; k++)
				color += get_color(r, 0);
			color /= samples;
			image[emitter.height / 2 - i - 1][j + emitter.width / 2] =
			  png::rgb_pixel(toInt(color.x), toInt(color.y), toInt(color.z));
		}
	}
	image.write(filename);
}
