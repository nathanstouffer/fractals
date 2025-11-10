#include <cmath>
#include <ctime>

#include <algorithm>
#include <complex>
#include <iostream>
#include <sstream>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stf/stf.hpp>

#include "fractalgen/generators/generators.hpp"

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
int static constexpr HEIGHT = (int)(WIDTH*(TOP-BOTTOM)/(RIGHT-LEFT));

// not diverging color (should be in 255 format)
#define CONV 0,0,0
// diverging color
#define RED 0
#define GREEN 0.4
#define BLUE 0.1

namespace fractalgen
{

    enum class Types
    {
        MANDELBROT,
        TEARDROP,
        POWERTOWER,
        NEWTON,
    };

}

int main(int argc, char** argv) {

    fractalgen::Types option = fractalgen::Types::MANDELBROT;

    stfd::vec2 min(LEFT, BOTTOM);
    stfd::vec2 max(RIGHT, TOP);
    stfd::aabb2 bounds(min, max);
    fractalgen::generators::generator::window_t window = { bounds, WIDTH };

    fractalgen::rgb_t conv = { CONV };

    // Mandelbrot set
    fractalgen::generators::mandelbrot mand(conv, RED, GREEN, BLUE);
    // Teardrop (inverted mandelbrot) - we rotate around the x-axis by theta
    double theta = stfd::constants::pi;
    fractalgen::generators::teardrop teardrop(theta, conv, RED, GREEN, BLUE);
    // tetration (power tower)
    fractalgen::generators::powertower power(conv, RED, GREEN, BLUE);
    // newton's method in the complex plane
    fractalgen::generators::newton newt(conv);

    // set up fractal gen pointer
    std::string path;

    // generate the fractal
    std::vector<fractalgen::rgb_t> pixels;
    switch (option)
    {
        case fractalgen::Types::MANDELBROT:
            std::cout << "Starting mandelbrot" << std::endl;
            pixels = mand.generate(window);
            path = "../../../../img/mandelbrot/mandelbrot.png";
            std::cout << "Success for mandelbrot" << std::endl;
            break;
        case fractalgen::Types::TEARDROP:
            std::cout << "Starting teardrop" << std::endl;
            pixels = teardrop.generate(window);
            path = "../../../../img/teardrop/teardrop.png";
            std::cout << "Success for teardrop" << std::endl;
            break;
        case fractalgen::Types::POWERTOWER:
            std::cout << "Starting power tower" << std::endl;
            pixels = power.generate(window);
            path = "../../../../img/powertower/powertower.png";
            std::cout << "Success for power tower" << std::endl;
            break;
        case fractalgen::Types::NEWTON:
            std::cout << "Starting newton" << std::endl;
            pixels = newt.generate(window);
            path = "../../../../img/newton/newton.png";
            std::cout << "Success for newton" << std::endl;
            break;
        default:
            std::cout << "No option selected" << std::endl;
    }

    // save to png
    {
        std::vector<unsigned char> bytes;
        bytes.reserve(3 * pixels.size());
        std::for_each(pixels.begin(), pixels.end(), [&bytes](fractalgen::rgb_t const& c) { bytes.push_back(c.r); bytes.push_back(c.g); bytes.push_back(c.b); });
        int result = stbi_write_png("fractal.png", window.width, window.height, 3, bytes.data(), window.width * 3);
    }

    return 0;
}
