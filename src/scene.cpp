#include <algorithm>
#include <random>
#include <png++/image.hpp>
#include <png++/png.hpp>
#include "scene.h"

using namespace std;

Camera::Camera(const Ray &cam, int w, int h): camera(cam), width(w), height(h)
{
	plane_x = camera.dir.get_orthogonal();
	plane_y = vector_prod(camera.dir, plane_x).normalize();
	double normal_coeff = height_absolute * camera.dir.length() / height;
	plane_y = plane_y * normal_coeff;
	plane_x = plane_x * normal_coeff;
}

Intersection Scene::intersect(const Ray &r)
{
	Intersection res;
	double curr_min = 1000.; // FIXME hack
	for (auto &obj : objects) {
		Intersection inter = obj->intersect(r);
		if (inter.time && inter.time < curr_min) {
			curr_min = inter.time;
			res = inter;
		}
	}

	return res;
}
bool depth = false;
mt19937::result_type seed = 0;
mt19937 gen(seed);
uniform_real_distribution<double> distr(0., 1.);
inline double ddrand()
{
	return 2 * drand48() - 1.;
}

Color Scene::get_color(const Ray &r, int depth)
{
	if (!depth) {
		return {};
	}

	Intersection i = intersect(r);

	// Didn't hit anything
	if (i.time == 0) {
		return {};
	}

	if (i.material.light)
		return {12., 12., 12.};
	vec3 u = i.ray.dir.get_orthogonal();
	vec3 v = vector_prod(i.ray.dir, u);
	i.ray.dir = ddrand() * i.ray.dir + ddrand() * u + ddrand() * v;
	return mult(i.material.color, get_color(i.ray, depth - 1));
}

void Scene::add_cube(vec3 top_dir, const Triangle &bottom,
                     const std::array<Material, 6> &sides)
{
	vec3 A = bottom.getA();
	vec3 B = bottom.getB();
	vec3 C = bottom.getC();
	vec3 D = bottom.getC() + bottom.getB() - bottom.getA();
	objects.push_back(unique_ptr<Object>(new Triangle(A, B, C, sides[Bottom])));
	objects.push_back(unique_ptr<Object>(new Triangle(C, B, D, sides[Bottom])));

	// Coords of top rectangle
	vec3 A_ = A + top_dir;
	vec3 B_ = B + top_dir;
	vec3 C_ = C + top_dir;
	vec3 D_ = D + top_dir;

	objects.push_back(unique_ptr<Object>(new Triangle(A, B, A_, sides[Left])));
	objects.push_back(unique_ptr<Object>(new Triangle(A_, B_, B, sides[Left])));

	objects.push_back(unique_ptr<Object>(new Triangle(A, C, A_, sides[Back])));
	objects.push_back(unique_ptr<Object>(new Triangle(A_, C_, C, sides[Back])));

	objects.push_back(unique_ptr<Object>(new Triangle(B, D, B_, sides[Front])));
	objects.push_back(unique_ptr<Object>(new Triangle(B_, D_, D, sides[Front])));

	objects.push_back(unique_ptr<Object>(new Triangle(C, D, C_, sides[Right])));
	objects.push_back(unique_ptr<Object>(new Triangle(D_, C_, D, sides[Right])));

	objects.push_back(unique_ptr<Object>(new Triangle(A_, B_, C_, sides[Top])));
	objects.push_back(unique_ptr<Object>(new Triangle(B_, C_, D_, sides[Top])));
}

inline int toInt(double x)
{
	return int(pow(std::clamp(x, 0., 1.), 1 / 2.2) * 255 + .5);
}

void Scene::render_scene(string filename, int samples)
{
	png::image<png::rgb_pixel> image(camera.width, camera.height);
	Ray r;
	r.start = camera.camera.start;
	for (int i = -camera.height / 2; i < camera.height / 2; i++) {
		for (int j = -camera.width / 2; j < camera.width / 2; j++) {
			r.dir = camera.camera.dir + camera.plane_x * j + camera.plane_y * i;
			Color color;
			for (auto i = 0; i < samples; i++)
				color += get_color(r, max_depth);
			color /= samples;
			image[camera.height / 2 - i - 1][j + camera.width / 2] =
			  png::rgb_pixel(toInt(color.x), toInt(color.y), toInt(color.z));
		}
	}
	image.write(filename);
}
