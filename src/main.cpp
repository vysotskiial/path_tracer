#include <iostream>
#include "scene.h"

using namespace std;

int main(int argc, char *argv[])
{
	Camera c({{}, {1, 0, 0}}, 512, 512);
	Scene s(c);
	s.add_object(
	  unique_ptr<Object>(new Sphere({4, 0, -0.7}, .3, vec3(1., 1., 0.))));
	// TODO addcube method to scene
	// Back
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, 1}, {5, 1, 1}, {5, 1, -1}, vec3(1., 0., 0.))));
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, 1}, {5, -1, -1}, {5, 1, -1}, vec3(1., 0., 0.))));
	// Left
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, 1}, {3, -1, 1}, {3, -1, -1}, vec3(0., 0., 1.))));
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, 1}, {5, -1, -1}, {3, -1, -1}, vec3(0., 0., 1.))));
	// Right
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, 1, 1}, {3, 1, 1}, {3, 1, -1}, vec3(0., 1., 0.))));
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, 1, 1}, {5, 1, -1}, {3, 1, -1}, vec3(0., 1., 0.))));
	// Top
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, 1}, {3, -1, 1}, {3, 1, 1}, vec3(1., 1., 1.))));
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, 1}, {5, 1, 1}, {3, 1, 1}, vec3(1., 1., 1.))));
	// Bottom
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, -1}, {3, -1, -1}, {3, 1, -1}, vec3(0., 1., 1.))));
	s.add_object(unique_ptr<Object>(
	  new Triangle({5, -1, -1}, {5, 1, -1}, {3, 1, -1}, vec3(0., 1., 1.))));
	// s.add_object(unique_ptr<Object>(new Sphere({3, 2, 3}, 1., vec3(1., 0.,
	// 0.))));*/
	s.render_scene("abd", 0);
	return 0;
}
