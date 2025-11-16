#include <cmath>
#include <ctime>

#include <algorithm>
#include <complex>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>

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

    int generate(generators::types type)
    {
        // set up fractal config
        generators::config config = 
        {
            type,
            { 0, 0, 0 },
            { 0, 100, 25 },
            0.0
        };

        // set up window
        stfd::vec2 min(LEFT, BOTTOM);
        stfd::vec2 max(RIGHT, TOP);
        stfd::aabb2 bounds(min, max);
        generators::window_t window = { bounds, WIDTH };

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

        using types = generators::types;

        std::map<std::string, types> types_map = 
        {
            {"mandelbrot", types::mandelbrot},
            {"rotated_mandelbrot", types::rotated_mandelbrot},
            {"powertower", types::powertower},
            {"newton", types::newton}
        };

        generators::types type;
        app.add_option("-t,--type", type, "Select type")
            ->required()
            ->transform(CLI::CheckedTransformer(types_map));

        CLI11_PARSE(app, argc, argv);

        return generate(type);
    }

}

int main(int argc, char** argv)
{
    return fractalgen::main(argc, argv);
}
