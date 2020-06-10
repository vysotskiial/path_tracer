#include <iostream>
#include <getopt.h>
#include <boost/program_options.hpp>
#include "scene.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
	// clang-format off
	po::options_description options("Options");
	options.add_options()
		("help", "Produce this message")
		("width,w", po::value<int>()->default_value(512), "Width of image in pixels")
		("height,h", po::value<int>()->default_value(512), "Height of image in pixels")
		("samples,s", po::value<int>()->default_value(100), "Number of samples per pixel")
		("output,o", po::value<string>()->default_value("image"s), "Output file name");
	// clang-format on
	po::variables_map args;
	try {
		po::store(po::parse_command_line(argc, argv, options), args);
	}
	catch (std::exception &e) {
		cerr << "Error: " << e.what() << "\n";
		cerr << options << "\n";
		return 1;
	}

	if (args.count("help")) {
		cout << options << "\n";
		return 1;
	}

	Camera c({{3.1, 0, 0}, {0.1, 0, 0}}, args["width"].as<int>(),
	         args["height"].as<int>());
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
	s.add_object(make_unique<Sphere>(vec3{5.5, .5, -.5}, .5, mirror));

	Material light;
	light.light = true;
	s.add_object(make_unique<Circle>(vec3{5.3, 0., 0.99}, vec3{5.6, 0., 0.99},
	                                 vec3{5.3, .3, 0.99}, light));
	s.render_scene(args["output"].as<string>() + ".png"s,
	               args["samples"].as<int>());
	return 0;
}
