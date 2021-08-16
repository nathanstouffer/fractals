#version 330 core

in vec2 myFragPos;
in vec3 myColor;
out vec4 fragColor;

float modulus(vec2 num) { return sqrt(num.x*num.x + num.y*num.y); }
vec2 square(vec2 num) { return vec2(num.x*num.x - num.y*num.y, 2*num.x*num.y); }

float fmod(float x, float r) {
    x = abs(x);  r = abs(r);
    while (x >= r) { x = x - r; }
    return x;
}

vec3 hsv_to_rgb(float h, float s, float v) {

    float H = 180.0 * h / 3.14159265635;
    float c = s * v;
    float x = c * (1.0 - abs(fmod(H/60.0, 2.0) - 1.0));
    float m = v - c;

    float r = 0.0, g = 0.0, b = 0.0;
    if (H < 60) {
        r = c; g = x; b = 0;
    }
    else if (60 <= H && H < 120) {
        r = x; g = c; b = 0;
    }
    else if (120 <= H && H < 180) {
        r = 0; g = c; b = x;
    }
    else if (180 <= H && H < 240) {
        r = 0; g = x; b = c;
    }
    else if (240 <= H && H < 300) {
        r = x; g = 0; b = c;
    }
    else if (300 <= H && H < 360) {
        r = c; g = 0; b = x;
    }

    return vec3(r, g, b);
}

void main() {
    float PI = 3.14159265635;
    vec3 bckgrnd = vec3(0.0, 0.0, 0.0);
    vec3 result  = myColor;
    vec2 num     = myFragPos.xy;

    float s = 0.9;
    float v = 0.8;

    // mandelbrot computations
    // quick check to decrease computation time
    if (modulus(num) < 0.2) { result = bckgrnd; }
    else if (num.x < 0 && modulus(num) < 0.6) {
        result = bckgrnd;
    }
    else {
        vec2 z = vec2(0.0, 0.0);                                        // start the 0-orbit
        int iter_cap = 500;                                             // set an iteration cap
        int mag_cap = 10000;                                            // set a magnitude cap
        int i;
        for (i = 0; i < iter_cap && modulus(z) <= mag_cap; i++) {       // iterate 0 on z_n+1 = z_n^2 + num
            z = square(z) + num;
        }
        if (i == iter_cap) { result = bckgrnd; }                        // if orbit has not diverged to infinity, return the background color
        else {                                                          // otherwise, compute the scaled color
            float f = 30*PI;

            float h = float(i) / f;                                     // compute h and turn into angle in [0, 2*pi]
            h = h - int(h);
            h *= 2.0*PI;
            result = hsv_to_rgb(h, s, v);                               // convert hsv to rgb type
        }
    }

    fragColor = vec4(result, 1.0f);
}
