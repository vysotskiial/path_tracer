#include <iostream>
#include <fstream>
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
		("threads,t", po::value<int>()->default_value(1), "Number of threads")
		("output,o", po::value<string>()->default_value("image"s), "Output file name")
		("input,i", po::value<string>()->required(), "Scene description file");
	// clang-format on
	po::variables_map args;
	try {
		po::store(po::parse_command_line(argc, argv, options), args);
		if (args.count("help")) {
			cout << options << "\n";
			return 1;
		}
		po::notify(args);
	}
	catch (std::exception &e) {
		cerr << "Error: " << e.what() << "\n";
		cerr << options << "\n";
		return 1;
	}

	try {
		Scene s(args["input"].as<string>(), args["width"].as<int>(),
		        args["height"].as<int>());
		s.render_scene(args["output"].as<string>() + ".png"s,
		               args["samples"].as<int>(), args["threads"].as<int>());
	}
	catch (std::exception &e) {
		cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
