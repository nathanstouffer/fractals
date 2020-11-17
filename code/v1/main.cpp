#include <iostream>
#include <sstream>

#include <bitmap/bitmap_image.hpp>
#include <cmath>
#include <complex>
#include <ctime>

#include "fractal.h"

// left, right, top, and bottom define the window of the complex plane that we view
#define LEFT -4.5
#define RIGHT 3
#define TOP 4
#define BOTTOM -4
// width and height define the resolution of the image (compute based on ratio between width/height of the rectangle)
#define WIDTH 1000
#define HEIGHT (int)(WIDTH*(TOP-BOTTOM)/(RIGHT-LEFT))
// converging color (should be in 255 format)
#define CONV 255,255,255
// diverging color
#define RED 0
#define GREEN 0
#define BLUE 0
// define options
#define MANDELBROT 0
#define POWERTOWER 1
#define NEWTON 2


void render(bitmap_image& image, FractalGen* generator) {
    double x_step = (double)(RIGHT - LEFT)/WIDTH;
    double y_step = (double)(TOP - BOTTOM)/HEIGHT;
    //std::cout << "x step: " << x_step << std::endl;
    //std::cout << "y step: " << y_step << std::endl;
    time_t start = std::time(NULL);                                             // get start time
    // loop over all pixels
    for (unsigned int i = 0; i < WIDTH; i++) {
        if (i % (WIDTH/10) == 0) {                                              // intermittent status update
            time_t current = std::time(NULL);                                   // get current time
            std::cout << (int)(100*i/(double)WIDTH) << "% complete  "
                    << current-start << " seconds passed" << std::endl;
        }
        for (unsigned int j = 0; j < HEIGHT; j++) {
            std::complex<double> num(LEFT + i*x_step, BOTTOM + j*y_step);
            // compute convergence
            rgb_t color = generator->color_complex_num(num);
            // set color
            image.set_pixel(i, j, color);
        }
    }
}

int main(int argc, char** argv) {

    int option = POWERTOWER;

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(WIDTH, HEIGHT);
    rgb_t conv = make_colour(CONV);

    // Mandelbrot set
    Mandelbrot mand(conv, RED, GREEN, BLUE);
    // tetration (power tower)
    PowerTower power(conv, RED, GREEN, BLUE);

    // set up fractal gen pointer
    FractalGen* fg;

    // render the fractal
    switch (option) {
        case MANDELBROT:
            fg = &mand;
            std::cout << "Starting mandelbrot" << std::endl;
            render(image, fg);
            image.save_image("../../../img//mandelbrot/mandelbrot.bmp");
            std::cout << "Success for mandelbrot" << std::endl;
            break;
        case POWERTOWER:
            fg = &power;
            std::cout << "Starting power tower" << std::endl;
            render(image, fg);
            image.save_image("../../../img/powertower/powertower.bmp");
            std::cout << "Success for power tower" << std::endl;
            break;
        case NEWTON:
            render(image, fg);
            std::cout << "String newton" << std::endl;
            image.save_image("../../../img/newton/newton.bmp");
            std::cout << "Success for newton" << std::endl;
            break;
        default:
            std::cout << "No option selected" << std::endl;
    }
    return 0;
}
