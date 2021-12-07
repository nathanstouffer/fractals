// interface IFractalGen
// with method color_complex_num()

#include <cmath>
#include <float.h>

/* Abstract class that generates a fractal by returning the appropriate color of a
 * complex input
*/
class FractalGen {
    public:

        //FractalGen() {};
        //~FractalGen() {};

        virtual rgb_t color_complex_num(std::complex<double> num) = 0;

        // assumptions:
        //     --- h \in [0, 2*pi]
        //     --- s \in [0, 1]
        //     --- v \in [0, 1]
        rgb_t hsv_to_rgb(double h, double s, double v) {
            double H = 180.0 * h / M_PI;
            double c = s * v;
            double x = c * (1 - fabs(fmod(H/60.0, 2) - 1));
            double m = v - c;
            //std::cout << "c: " << c << " x: " << x << " m: " << m << std::endl;

            double r = 0, g = 0, b = 0;
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
            //std::cout << "r: " << r << " g: " << g << " b: " << b << std::endl;

            int R = (r+m)*255;
            int G = (g+m)*255;
            int B = (b+m)*255;
            return make_colour(R, G, B);
        }
};

/* class that colors a complex number c according to the iterative rule z_n+1 = z_n + c
 * where z_0 = 0
 */
class Mandelbrot : public FractalGen {

private:

    rgb_t conv;
    double h_shift, f;
    double s, v;

public:

    // constructor
    Mandelbrot(rgb_t conv, double h_shift=0, double f=50*M_PI, double s=1, double v=1) {
        this->conv = conv;
        this->h_shift = h_shift;
        this->f = f;
        this->s = s;
        this->v = v;
    }

    rgb_t color_complex_num(std::complex<double> num) {
        // quick check to decrease computation time
        if (abs(num) < 0.2) { return this->conv; }
        else if (num.real() < 0) {
           if (abs(num) < 0.6) { return this->conv; }
        }

        std::complex<double> z(0.0, 0.0);                                   // start the 0-orbit
        int iter_cap = 20000;                                                // set an iteration cap
        int mag_cap = 10000;                                               // set a magnitude cap
        int i;
        for (i = 0; i < iter_cap && abs(z) <= mag_cap; i++) {               // iterate 0 on z_n+1 = z_n^2 + num
            z = pow(z, 2) + num;
        }
        if (i == iter_cap) { return this->conv; }                           // if orbit has not diverged to infinity, return the background color
        else {                                                              // otherwise, compute the appropriate color
            double h = (double)i / f;                                       // compute h and turn into angle in [0, 2*pi]
            h = h - (int)h;
            h *= 2.0*M_PI;
            h += h_shift;
            if (h > 2.0*M_PI) { h -= 2.0*M_PI; }
            return hsv_to_rgb(h, s, v);
        }
    }
};

/* class that colors a complex number c by transforming it to another point in the
 * complex plane (via mapping to the Riemann sphere, rotating, and then mapping
 * back to the complex plane) and coloring according to regular mandelbrot coloring
 */
class Teardrop : public FractalGen {
private:

    Mandelbrot mand;
    double theta;

    struct Vec3 {
        double x, y, z;

        // apply rotation about the x-axis via a rotation matrix
        Vec3 rotateX(double theta) {
            Vec3 rotated;
            rotated.x = this->x;
            rotated.y = std::cos(theta) * this->y - std::sin(theta) * this->z;
            rotated.z = std::sin(theta) * this->y + std::cos(theta) * this->z;
            return rotated;
        }
    };

    Vec3 complexToRiemannSphere(std::complex<double> num) {
        double x = num.real();
        double y = num.imag();

        double denom = 1 + x * x + y * y;
        Vec3 vec;
        vec.x = 2 * x / denom;
        vec.y = 2 * y / denom;
        vec.z = (-1 + x * x + y * y) / denom;
        return vec;
    }

    std::complex<double> riemannSphereToComplex(Vec3 vec) {
        if (vec.x == 0.0 && vec.y == 0.0 && vec.z == 1.0) {
            return std::complex<double>(DBL_MAX, DBL_MAX);
        }

        double a = vec.x / (1.0 - vec.z);
        double b = vec.y / (1.0 - vec.z);
        return std::complex<double>(a, b);
    }

public:

    // constructor
    Teardrop(double theta, rgb_t conv, double h_shift=0, double f=50*M_PI, double s=1, double v=1) : mand(Mandelbrot(conv, h_shift, f, s, v)) {
        this->theta = theta;
    }

    rgb_t color_complex_num(std::complex<double> num) {
        Vec3 riemannSpherePoint = complexToRiemannSphere(num);                      // map to riemann sphere
        Vec3 rotatedPoint = riemannSpherePoint.rotateX(2 * M_PI - this->theta);     // rotate by complement of theta since num is in the image space and we want the preimage
        std::complex<double> z = riemannSphereToComplex(rotatedPoint);              // map back to the complex plane
        return mand.color_complex_num(z);
    }
};

/* class that colors a complex number c according to the iterative rule z_n+1 = num^z_n
 * where z_0 = num
 */
class PowerTower : public FractalGen {

private:

    rgb_t conv;
    double h_shift, f;
    double s, v;

public:

    // constructor
    PowerTower(rgb_t conv, double h_shift=0, double f=50*M_PI, double s=1, double v=1) {
        this->conv = conv;
        this->h_shift = h_shift;
        this->f = f;
        this->s = s;
        this->v = v;
    }

    rgb_t color_complex_num(std::complex<double> num){
        std::complex<double> z = num;                                       // start the input
        std::complex<double> prev = z;                                      // set up previous value
        int iter_cap = 1000;                                                // set an iteration cap
        double mag_cap = 1000;                                              // set up a magnitude cap
        int i;
        for (i = 0; i < iter_cap && abs(z) < mag_cap; i++) {                // iterate 0 on z_n+1 = num^z_n
            z = pow(num, z);                                                // exponentiate
        }
        if (i == iter_cap) { return this->conv; }                           // if orbit has not diverged to infinity, return the background color
        else {                                                              // otherwise, compute the scaled color
            double h = (double)i / f;                                       // compute h and turn into angle in [0, 2*pi]
            h = h - (int)h;
            h *= 2.0*M_PI;
            h += h_shift;
            if (h > 2.0*M_PI) { h -= 2.0*M_PI; }
            return hsv_to_rgb(h, s, v);
        }
    }

};
