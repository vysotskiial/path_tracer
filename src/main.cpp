#include <iostream>
#include <getopt.h>
#include "scene.h"

using namespace std;

void usage(const char *name)
{
	cerr << "Usage: " << name << " <options>\n";
	cerr << "Options:\n";
	cerr << "\t --width | -w <width>        set image width\n";
	cerr << "\t --height | -h <height>      set image height\n";
	cerr << "\t --samples | -s <samples>    set number of samples per pixel\n";
	cerr << "\t --output | -o <filename>    set output file name\n";
}

int main(int argc, char *argv[])
{
	unsigned width = 512;
	unsigned height = 512;
	unsigned samples = 100;
	string output_file = "image"s;

	constexpr option long_options[] = {
	  {"width", required_argument, NULL, 'w'},
	  {"height", required_argument, NULL, 'h'},
	  {"samples", required_argument, NULL, 's'},
	  {"output", required_argument, NULL, 'o'},
	  {0, 0, 0, 0},
	};

	while (true) {
		char c = getopt_long(argc, argv, "w:h:s:o:", long_options, NULL);
		if (c == -1)
			break;
		switch (c) {
		case 'w':
			width = stoi(optarg);
			break;
		case 'h':
			height = stoi(optarg);
			break;
		case 's':
			samples = stoi(optarg);
			break;
		case 'o':
			output_file = optarg;
			break;
		default:
			usage(argv[0]);
			return 1;
		}
	}

	Camera c({{3.1, 0, 0}, {0.1, 0, 0}}, width, height);
	Scene s(c);

	std::array<Material, 6> sides;
	sides[Top].color = {1., 1., 1.};
	sides[Bottom].color = {1., 1., 1.};
	sides[Left].color = {0.75, 0.25, 0.25};
	sides[Right].color = {0.25, 0.25, 0.75};
	sides[Front].color = {1., 1., 1.};
	sides[Back].color = {1., 1., 1.};

	Triangle bot({3., -1., -1.}, {6., -1, -1}, {3., 1., -1});
	s.add_cube({0., 0., 2.}, bot, sides);
	Material mirror;
	mirror.mirror = true;
	s.add_object(unique_ptr<Object>(new Sphere({5.5, .5, -.5}, .5, mirror)));

	Material light;
	light.light = true;
	Circle lamp({5.3, 0., 0.99}, {5.6, 0., 0.99}, {5.3, .3, 0.99}, light);
	s.add_object(unique_ptr<Object>(new Circle(lamp)));
	s.render_scene(output_file + ".png"s, samples);
	return 0;
}
