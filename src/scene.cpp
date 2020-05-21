#include <png++/image.hpp>
#include <png++/png.hpp>
#include "scene.h"

using namespace std;

Camera::Camera(const Ray &cam, int w, int h): camera(cam), width(w), height(h)
{
	camera.dir.normalize();
	plane_x = camera.dir.get_orthogonal();
	plane_y = vector_prod(camera.dir, plane_x).normalize();
	double normal_coeff = height_absolute / height;
	plane_y = plane_y * normal_coeff;
	plane_x = plane_x * normal_coeff;
}

Ray Scene::intersect(const Ray &r)
{
	Ray res;
	double curr_min = 1000.; // FIXME hack
	for (auto &obj : objects) {
		double time;
		Ray cur_ray = obj->intersect(r, time);
		if (time && time < curr_min) {
			curr_min = time;
			res = cur_ray;
		}
	}

	return res;
}

void Scene::render_scene(string filename, int samples_per_pixel)
{
	png::image<png::rgb_pixel> image(camera.width, camera.height);
	Ray r;
	r.start = camera.camera.start;
	for (int i = -camera.height / 2; i < camera.height / 2; i++) {
		for (int j = -camera.width / 2; j < camera.width / 2; j++) {
			r.dir = camera.camera.dir + camera.plane_x * j + camera.plane_y * i;
			vec3 color = intersect(r).color;
			image[camera.height / 2 - i - 1][j + camera.width / 2] =
			  png::rgb_pixel(color.x * 255, color.y * 255, color.z * 255);
		}
	}
	image.write("image.png");
}
