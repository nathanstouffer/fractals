#include <iostream>
#include <sstream>

#include <bitmap/bitmap_image.hpp>
#include <cmath>
#include <complex>
#include <ctime>

#include "fractal.h"

// left, right, top, and bottom define the window of the complex plane that we view
#define LEFT -2.5
#define RIGHT 1
#define TOP 1.25
#define BOTTOM -1.25
// width and height define the resolution of the image (compute based on ratio between width/height of the rectangle)
#define WIDTH 20000
#define HEIGHT (int)(WIDTH*(TOP-BOTTOM)/(RIGHT-LEFT))
// background color
#define BCKGRND 0,0,0
// fractal hue
#define RED 0.1
#define GREEN 0.3
#define BLUE 1


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

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(WIDTH, HEIGHT);
    rgb_t bckgrnd = make_colour(BCKGRND);

    Mandelbrot mand(bckgrnd, RED, GREEN, BLUE);

    FractalGen* fg = &mand;

    // render the fractal
    render(image, fg);
    image.save_image("../../../img/mandelbrot.bmp");
    std::cout << "Success for mandelbrot.bmp" << std::endl;
    return 0;
}
