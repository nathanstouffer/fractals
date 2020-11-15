#include <iostream>
#include <sstream>

#include <bitmap/bitmap_image.hpp>

#include <cmath>

#define WIDTH 640
#define HEIGHT 480
#define BCKGRND 66,252,190
#define EPSILON 0.0001

void render(bitmap_image& image) {
    // set bounding rectangle { l, r, b, t}
    std::vector<float> bounds{ -5, 5, -5, 5 };
    // loop over all pixels
    rgb_t background = make_colour(BCKGRND);
    for (unsigned int i = 0; i < WIDTH; i++) {
        for (unsigned int j = 0; j < HEIGHT; j++) {
            // compute convergence
            int rate = 0;
            // set color
            if (rate = 0) { image.set_pixel(i, j, background); }
            else {
                rgb_t color = make_colour(BCKGRND);
                image.set_pixel(i, j, color);
            }
        }
    }
}

int main(int argc, char** argv) {

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(WIDTH, HEIGHT);

    // render the world
    render(image);
    image.save_image("../../../img/test.bmp");
    std::cout << "Success for test.bmp" << std::endl;
}
