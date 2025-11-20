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
#include "fractalgen/options.hpp"

namespace fractalgen
{

    int generate(options const& opts)
    {
        std::unique_ptr<generators::generator> generator = generators::factory(opts.config());
        if (generator)
        {
            generators::window_t window = opts.window();
            std::vector<rgb_t> pixels = generator->generate(window);

            // save to png
            std::vector<unsigned char> bytes;
            bytes.reserve(3 * pixels.size());
            std::for_each(pixels.begin(), pixels.end(), [&bytes](rgb_t const& c) { bytes.push_back(c.r); bytes.push_back(c.g); bytes.push_back(c.b); });
            std::string filename = opts.name;
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

    void add_base_options(CLI::App& subcommand, options& opts)
    {
        subcommand.add_option("-n,--name", opts.name, "Name of the fractal (the output is written to name.png)")
            ->capture_default_str();

        subcommand.add_option("-w,--width", opts.width, "Width (in pixels) of the output image -- height is computed automatically")
            ->capture_default_str();

        subcommand.add_option("-b,--bounds", opts.bounds, "Bounds of the image in the complex plane. Format: min_x min_y max_x max_y")
            ->capture_default_str();

        subcommand.add_option("-p,--phi", opts.phi, "Angle (in radians) by which to rotate the Riemann Sphere about the y-axis")
            ->capture_default_str();
    }

    void add_mandelbrot(CLI::App& app, options& opts)
    {
        CLI::App* mandelbrot = app.add_subcommand("mandelbrot", "Render the mandelbrot set");
        mandelbrot->callback([&]() { opts.type = generators::types::mandelbrot; });
        add_base_options(*mandelbrot, opts);

        mandelbrot->add_option("-c,--color", opts.mandelbrot.color, "The color (0-255) assigned to non-diverging inputs. Format: R G B")
            ->type_name("R G B")
            ->capture_default_str();

        mandelbrot->add_option("-d,--diverging", opts.mandelbrot.diverging, "The color (0-255) assigned to diverging inputs. Format: R G B")
            ->type_name("R G B")
            ->capture_default_str();
    }

    void add_powertower(CLI::App& app, options& opts)
    {
        CLI::App* powertower = app.add_subcommand("powertower", "Render a power tower fractal");
        powertower->callback([&]() { opts.type = generators::types::powertower; });
        add_base_options(*powertower, opts);

        powertower->add_option("-c,--color", opts.powertower.color, "The color (0-255) assigned to non-diverging inputs. Format: R G B")
            ->type_name("R G B")
            ->capture_default_str();

        powertower->add_option("-d,--diverging", opts.powertower.diverging, "The color (0-255) assigned to diverging inputs. Format: R G B")
            ->type_name("R G B")
            ->capture_default_str();
    }

    void add_newton(CLI::App& app, options& opts)
    {
        CLI::App* newton = app.add_subcommand("newton", "Render a fractal using newton's method to find roots");
        newton->callback([&]() { opts.type = generators::types::newton; });
        add_base_options(*newton, opts);

        newton->add_option("-d,--diverging", opts.newton.diverging, "The color (0-255) assigned to diverging inputs. Format: R G B")
            ->type_name("R G B")
            ->capture_default_str();

        // TODO (stouff) possibly rename this to roots and think of a different greek letter for rotation
        newton->add_option("-z", opts.newton.roots, "")
            ->type_name("REAL IMAG R G B")
            ->expected(5);
    }

    int main(int argc, char** argv)
    {
        CLI::App app{"fractalgen is a tool that generates images by coloring the complex plane.", "fractalgen"};
        app.require_subcommand(1);

        options opts;
        add_mandelbrot(app, opts);
        add_powertower(app, opts);
        add_newton(app, opts);

        CLI11_PARSE(app, argc, argv);

        return generate(opts);
    }

}

int main(int argc, char** argv)
{
    return fractalgen::main(argc, argv);
}
