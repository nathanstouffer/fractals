#include <iostream>
#include <sstream>

#include <bitmap/bitmap_image.hpp>
#include <cmath>
#include <complex>
#include <ctime>
#include <thread>

#include "fractal.h"

// (X, Y) is the leftmost pixel halfway up the image and W is the width
// the height H is computed
#define X 1.75
#define Y 0
#define W 9
#define H W*(9.0/16.0)
// left, right, top, and bottom define the window of the complex plane that we view
#define LEFT (X-W/2.0)
#define RIGHT (X+W/2.0)
#define TOP (Y+H/2.0)
#define BOTTOM (Y-H/2.0)
// width and height define the resolution of the image (compute based on ratio between width/height of the rectangle)
// desktop backgrounds should have a width of 25000
#define WIDTH 4000
#define HEIGHT (int)(WIDTH*(TOP-BOTTOM)/(RIGHT-LEFT))
// not diverging color (should be in 255 format)
#define CONV 0,0,0
// diverging color
#define RED 0
#define GREEN 0.4
#define BLUE 0.1
// define options
#define MANDELBROT 0
#define TEARDROP 1
#define POWERTOWER 2
#define NEWTON 3
// number of threads
#define NUMTHREADS 10

/* for making backgrounds on the desktop, the ratio should be 1920:1080
 *  - a good window (of the complex plane) for the mandelbrot set is [-4,1.3333] and [-1.5,1.5]
*/

/* function to color the pixels in a column of the matrix.
 * the bounds are [min_j, max_j)
 */
void color_pixels(bitmap_image* image, FractalGen* generator, unsigned int i, int min_j, int max_j){
    double x_step = (double)(RIGHT - LEFT)/WIDTH;
    double y_step = (double)(TOP - BOTTOM)/HEIGHT;
    for (unsigned int j = min_j; j < max_j; j++) {
        std::complex<double> num(LEFT + i*x_step, TOP - j*y_step);
        // compute convergence
        rgb_t color = generator->color_complex_num(num);
        // set color
        image->set_pixel(i, j, color);
    }
}

void render(bitmap_image* image, FractalGen* generator) {
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
        std::vector<std::thread> threads;
        for (unsigned int t = 0; t < NUMTHREADS; t++) {
            int min = (int)(t/(double)NUMTHREADS*HEIGHT);
            int max = (int)((t+1)/(double)NUMTHREADS*HEIGHT);
            threads.push_back(std::thread(color_pixels, image, generator, i, min, max));
        }
        for (unsigned int t = 0; t < NUMTHREADS; t++) { threads[t].join(); }
    }
}

int main(int argc, char** argv) {

    int option = TEARDROP;

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
    FractalGen* fg;

    // render the fractal
    switch (option) {
        case MANDELBROT:
            fg = &mand;
            std::cout << "Starting mandelbrot" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/mandelbrot/mandelbrot.bmp");
            std::cout << "Success for mandelbrot" << std::endl;
            break;
        case TEARDROP:
            fg = &teardrop;
            std::cout << "Starting teardrop" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/teardrop/teardrop.bmp");
            std::cout << "Success for teardrop" << std::endl;
            break;
        case POWERTOWER:
            fg = &power;
            std::cout << "Starting power tower" << std::endl;
            render(image, fg);
            image->save_image("../../../../img/powertower/powertower.bmp");
            std::cout << "Success for power tower" << std::endl;
            break;
        case NEWTON:
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
