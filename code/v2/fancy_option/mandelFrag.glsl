#version 330 core

in vec3 ourColor;
in vec3 ourFragPos;

out vec4 fragColor;

float modulus(vec2 num) { return sqrt(num.x*num.x + num.y*num.y); }
vec2 square(vec2 num) { return vec2(num.x*num.x - num.y*num.y, 2*num.x*num.y); }

void main() {
    vec3 bckgrnd = vec3(0.0, 0.0, 0.0);
    vec3 result  = ourColor;
    vec2 num     = ourFragPos.xy;
    // mandelbrot computations
    // quick check to decrease computation time
    if (modulus(num) < 0.2) { result = bckgrnd; }
    else if (num.x < 0){
        if (modulus(num) < 0.6) { result = bckgrnd; }
    }
    else {
        vec2 z = vec2(0.0, 0.0);                                            // start the 0-orbit
        int cap = 500;                                                      // set an iteration cap
        int i;
        for (i = 0; i < cap && modulus(z) <= 2.0; i++) {                    // iterate 0 on z_n+1 = z_n^2 + num
            z = square(z) + num;
        }
        if (modulus(z) <= 2) { result = bckgrnd;  }                         // if orbit has not diverged to infinity, return the background color
        else {                                                              // otherwise, compute the scaled color
            //double div = cap/(double)i;
            //int r = (int) 255*(this->r + (1/div)*(1-this->r));
            //int g = (int) 255*(this->g + (1/div)*(1-this->g));
            //int b = (int) 255*(this->b + (1/div)*(1-this->b));
            //rgb_t color = make_colour(r, g, b);
        }
    }

    fragColor = vec4(result, 1.0f);
}
