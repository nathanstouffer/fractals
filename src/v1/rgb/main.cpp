#include <iostream>
#include <sstream>

#include <bitmap/bitmap_image.hpp>
#include <cmath>
#include <complex>
#include <ctime>
#include <thread>

#include "fractal.h"

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
int static constexpr WIDTH = 4000;
int static constexpr HEIGHT = (int)(WIDTH*(TOP-BOTTOM)/(RIGHT-LEFT));

// number of threads
static constexpr size_t NUMTHREADS = 16;

static constexpr double X_STEP = (double)(RIGHT - LEFT) / WIDTH;
static constexpr double Y_STEP = (double)(TOP - BOTTOM) / HEIGHT;

// the resolution of the lattice used to sample each pixel
static constexpr size_t SAMPLES_RESOLUTION = 8;
static constexpr double INSET = 1.0 / (SAMPLES_RESOLUTION + 1);
static constexpr double X_PIXEL_INSET = INSET * X_STEP;
static constexpr double Y_PIXEL_INSET = INSET * Y_STEP;

enum class Types
{
    MANDELBROT,
    TEARDROP,
    POWERTOWER,
    NEWTON,
};

// not diverging color (should be in 255 format)
#define CONV 0,0,0
// diverging color
#define RED 0
#define GREEN 0.4
#define BLUE 0.1

rgb_t pixel_color(FractalGen* generator, unsigned int i, unsigned int j)
{
    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;
    double intial_x = LEFT + i * X_STEP + X_PIXEL_INSET;
    double intial_y = TOP  - j * Y_STEP + Y_PIXEL_INSET;
    for (size_t u = 0; u < SAMPLES_RESOLUTION; ++u)
    {
        for (size_t v = 0; v < SAMPLES_RESOLUTION; ++v)
        {
            double x = intial_x + u * X_PIXEL_INSET;
            double y = intial_y + v * Y_PIXEL_INSET;
            std::complex<double> num(x, y);
            rgb_t color = generator->color_complex_num(num);
            r += color.red;
            g += color.green;
            b += color.blue;
        }
    }
    r /= (SAMPLES_RESOLUTION * SAMPLES_RESOLUTION);
    g /= (SAMPLES_RESOLUTION * SAMPLES_RESOLUTION);
    b /= (SAMPLES_RESOLUTION * SAMPLES_RESOLUTION);
    return make_colour(r, g, b);
}

/**
 * function to color the pixels in a row of the matrix. the bounds are [min_j, max_j)
 */
void color_pixels(bitmap_image* image, FractalGen* generator, unsigned int i, int min_j, int max_j)
{
    for (unsigned int j = min_j; j < max_j; j++)
    {
        rgb_t color = pixel_color(generator, i, j);
        image->set_pixel(i, j, color);
    }
}

void render(bitmap_image* image, FractalGen* generator) {
    time_t start = std::time(NULL);                                             // get start time
    // loop over all pixels
    for (unsigned int i = 0; i < WIDTH; i++)
    {
        if (i % (WIDTH/10) == 0)                                                // intermittent status update
        {
            time_t current = std::time(NULL);                                   // get current time
            std::cout << (int)(100*i/(double)WIDTH) << "% complete  " << current-start << " seconds passed" << std::endl;
        }
        std::vector<std::thread> threads;
        for (unsigned int t = 0; t < NUMTHREADS; ++t)
        {
            int min = (int)(t/(double)NUMTHREADS*HEIGHT);
            int max = (int)((t+1)/(double)NUMTHREADS*HEIGHT);
            threads.push_back(std::thread(color_pixels, image, generator, i, min, max));
        }
        for (unsigned int t = 0; t < NUMTHREADS; ++t) { threads[t].join(); }
    }
}

int main(int argc, char** argv) {

    Types option = Types::MANDELBROT;

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image tmp(WIDTH, HEIGHT);
    bitmap_image* image = &tmp;
    rgb_t conv = make_colour(CONV);

    // Mandelbrot set
    Mandelbrot mand(conv, RED, GREEN, BLUE);
    // Teardrop (inverted mandelbrot) - we rotate around the x-axis by theta
    double theta = M_PI;
    Teardrop teardrop(theta, conv, RED, GREEN, BLUE);
    // tetration (power tower)
    PowerTower power(conv, RED, GREEN, BLUE);
    // newton's method in the complex plane
    Newton newt(conv);

    // set up fractal gen pointer
    FractalGen* fg = nullptr;

    // render the fractal
    switch (option) {
        case Types::MANDELBROT:
            fg = &mand;
            std::cout << "Starting mandelbrot" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/mandelbrot/mandelbrot.bmp");
            std::cout << "Success for mandelbrot" << std::endl;
            break;
        case Types::TEARDROP:
            fg = &teardrop;
            std::cout << "Starting teardrop" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/teardrop/teardrop.bmp");
            std::cout << "Success for teardrop" << std::endl;
            break;
        case Types::POWERTOWER:
            fg = &power;
            std::cout << "Starting power tower" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/powertower/powertower.bmp");
            std::cout << "Success for power tower" << std::endl;
            break;
        case Types::NEWTON:
            fg = &newt;
            std::cout << "Starting newton" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/newton/newton.bmp");
            std::cout << "Success for newton" << std::endl;
            break;
        default:
            std::cout << "No option selected" << std::endl;
    }
    return 0;
}
