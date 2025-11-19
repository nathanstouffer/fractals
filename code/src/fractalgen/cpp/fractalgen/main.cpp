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

    struct options
    {
        generators::config config;
        std::string name = "fractal.png";
        std::array<double, 4> bounds = { -4, -1.5, 1.33, 1.5 };
        int width = 750;
        double rho = 0.0;

        generators::window_t window() const
        {
            return { stfd::aabb2(stfd::vec2(bounds[0], bounds[1]), stfd::vec2(bounds[2], bounds[3])), width };
        }

    };

    void add_base_options(CLI::App& subcommand, options& opts)
    {
        subcommand.add_option("-n,--name", opts.name, "Name of the fractal (the output is written to name.png)")
            ->capture_default_str();

        subcommand.add_option("-w,--width", opts.width, "Width (in pixels) of the output image -- height is computed automatically")
            ->capture_default_str();

        subcommand.add_option("-b,--bounds", opts.bounds, "Bounds of the image in the complex plane. Format: min_x min_y max_x max_y")
            ->capture_default_str();

        subcommand.add_option("-r,--rho", opts.rho, "Angle (in radians) by which to rotate the Riemann Sphere about the y-axis")
            ->capture_default_str();
    }

    int main(int argc, char** argv)
    {
        CLI::App app{"fractalgen is a tool that generates images by coloring the complex plane.", "fractalgen"};
        app.fallthrough();

        using types = generators::types;
        options opts;

        CLI::App* mandelbrot = app.add_subcommand("mandelbrot", "Render the mandelbrot set");
        mandelbrot->callback([&]() { opts.config.type = types::mandelbrot; });
        add_base_options(*mandelbrot, opts);

        CLI::App* powertower = app.add_subcommand("powertower", "Render a power tower fractal");
        powertower->callback([&]() { opts.config.type = types::powertower; });
        add_base_options(*powertower, opts);

        CLI::App* newton = app.add_subcommand("newton", "Render a fractal using newton's method to find roots");
        newton->callback([&]() { opts.config.type = types::newton; });
        add_base_options(*newton, opts);

        app.require_subcommand(1);

        CLI11_PARSE(app, argc, argv);

        return generate(opts.config, opts.window(), opts.name);
    }

}

int main(int argc, char** argv)
{
    return fractalgen::main(argc, argv);
}
