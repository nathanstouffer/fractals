#include <cmath>
#include <ctime>

#include <algorithm>
#include <complex>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>
#include <tuple>

#include <CLI/CLI.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stf/stf.hpp>

#include "fractalgen/generators/generators.hpp"
#include "fractalgen/generators/factory.hpp"

namespace fractalgen
{

    int generate(generators::config const& config, generators::window_t const& window, std::string const& name)
    {
        std::unique_ptr<generators::generator> generator = generators::factory(config);
        if (generator)
        {
            std::vector<rgb_t> pixels = generator->generate(window);

            // save to png
            std::vector<unsigned char> bytes;
            bytes.reserve(3 * pixels.size());
            std::for_each(pixels.begin(), pixels.end(), [&bytes](rgb_t const& c) { bytes.push_back(c.r); bytes.push_back(c.g); bytes.push_back(c.b); });
            std::string filename = name;
            std::string suffix = ".png";
            if (!filename.ends_with(suffix))
            {
                filename += suffix;
            }
            int status = stbi_write_png(filename.c_str(), window.width, window.height, 3, bytes.data(), window.width * 3);
            bool success = status != 0;
            if (!success) { return 1; }
        }
        return 0;
    }

    int main(int argc, char** argv)
    {
        CLI::App app{"fractalgen is a tool that generates images by coloring the complex plane.", "fractalgen"};
        app.fallthrough();

        generators::config config;
        std::string name = "fractal";
        std::array<double, 4> bounds = { -4, -1.5, 1.33, 1.5 };
        int width = 750;
        double rho = 0.0;

        // set up window options
        {
            app.add_option("-n,--name", name, "Name of the fractal (the output is written to name.png)")
                ->capture_default_str();

            app.add_option("-w,--width", width, "Width (in pixels) of the output image -- height is computed automatically")
                ->capture_default_str();

            app.add_option("-b,--bounds", bounds, "Bounds of the image in the complex plane. Format: min_x min_y max_x max_y")
                ->capture_default_str();

            app.add_option("-r,--rho", rho, "Angle (in radians) by which to rotate the Riemann Sphere about the y-axis")
                ->capture_default_str();
        }

        using types = generators::types;

        CLI::App* mandelbrot = app.add_subcommand("mandelbrot"); mandelbrot->callback([&]() { config.type = types::mandelbrot; });
        CLI::App* powertower = app.add_subcommand("powertower"); powertower->callback([&]() { config.type = types::powertower; });
        CLI::App* newton     = app.add_subcommand("newton"    ); newton    ->callback([&]() { config.type = types::newton;     });

        app.require_subcommand(1);

        CLI11_PARSE(app, argc, argv);

        generators::window_t window = { stfd::aabb2(stfd::vec2(bounds[0], bounds[1]), stfd::vec2(bounds[2], bounds[3])), width };

        return generate(config, window, name);
    }

}

int main(int argc, char** argv)
{
    return fractalgen::main(argc, argv);
}
