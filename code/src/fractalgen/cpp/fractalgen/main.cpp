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

/**
 * for making backgrounds on the desktop, the ratio should be 1920:1080
 *  - a good window (of the complex plane) for the mandelbrot set is [-4,1.3333] and [-1.5,1.5]
 */

// (X, Y) is the leftmost pixel halfway up the image and W is the width
// the height H is computed
double static constexpr X = -1.333333333;
double static constexpr Y = 0;
double static constexpr W = 5.33333333;
double static constexpr H = W*(9.0/16.0);
// left, right, top, and bottom define the window of the complex plane that we view
double static constexpr LEFT = (X-W/2.0);
double static constexpr RIGHT = (X+W/2.0);
double static constexpr TOP = (Y+H/2.0);
double static constexpr BOTTOM = (Y-H/2.0);
// width and height define the resolution of the image (compute based on ratio between width/height of the rectangle)
// desktop backgrounds should have a width of 25000
int static constexpr WIDTH = 750;

namespace fractalgen
{

    int generate(generators::config const& config, generators::window_t const& window)
    {
        std::unique_ptr<generators::generator> generator = generators::factory(config);
        if (generator)
        {
            std::vector<rgb_t> pixels = generator->generate(window);

            // save to png
            std::vector<unsigned char> bytes;
            bytes.reserve(3 * pixels.size());
            std::for_each(pixels.begin(), pixels.end(), [&bytes](rgb_t const& c) { bytes.push_back(c.r); bytes.push_back(c.g); bytes.push_back(c.b); });
            return stbi_write_png("fractal.png", window.width, window.height, 3, bytes.data(), window.width * 3);
        }
        return 0;
    }

    int main(int argc, char** argv)
    {
        CLI::App app{"fractalgen is a tool that generates images by coloring the complex plane.", "fractalgen"};
        app.fallthrough();

        std::array<double, 4> bounds = { -4, -1.5, 1.33, 1.5 };
        int width = 750;
        double rho = 0.0;

        // set up window options
        {
            app.add_option("-w,--width", width, "Width (in pixels) of the output image -- height is computed automatically")
                ->capture_default_str();

            app.add_option("-b,--bounds", bounds, "Bounds of the image in the complex plane. Format: min_x min_y max_x max_y")
                ->capture_default_str();

            app.add_option("-r,--rho", rho, "Angle (in radians) by which to rotate the Riemann Sphere about the y-axis")
                ->capture_default_str();
        }

        using types = generators::types;

        CLI::App* mandelbrot = app.add_subcommand("mandelbrot");
        CLI::App* powertower = app.add_subcommand("powertower");
        CLI::App* newton = app.add_subcommand("newton");

        app.require_subcommand(1);

        CLI11_PARSE(app, argc, argv);

        generators::window_t window = { stfd::aabb2(stfd::vec2(bounds[0], bounds[1]), stfd::vec2(bounds[2], bounds[3])), width };

        // set up fractal config
        generators::config config = 
        {
            types::mandelbrot,
            { 0, 0, 0 },
            { 0, 100, 25 },
            rho
        };

        return generate(config, window);
    }

}

int main(int argc, char** argv)
{
    return fractalgen::main(argc, argv);
}
