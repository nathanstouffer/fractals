#pragma once

#include <cfloat>
#include <cmath>

#include <complex>
#include <iostream>

#include <stf/math/transform.hpp>
#include <stf/stf.hpp>
namespace fractalgen
{

    struct rgb_t
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        rgb_t() = default;
        rgb_t(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
        rgb_t(int _r, int _g, int _b) : r(static_cast<unsigned char>(_r)), g(static_cast<unsigned char>(_g)), b(static_cast<unsigned char>(_b)) {}

    };

    /**
     * Interface that provides a function to color an element of the complex plane
     */
    class generator
    {
    public:

        virtual ~generator() = default;

        virtual rgb_t color_complex_num(std::complex<double> const& num) const = 0;
    };

    /**
     * class that colors a complex number according to the iterative rule z_n+1 = (z_n)^2 + c
     */
    class mandelbrot : public generator
    {
    private:

        rgb_t conv;
        double r, g, b;

    public:

        // constructor
        mandelbrot(rgb_t _conv, double _r=1, double _g=1, double _b=1)
            : conv(_conv)
            , r(_r)
            , g(_g)
            , b(_b)
        {}

        rgb_t color_complex_num(std::complex<double> const& num) const override
        {
            // quick check to decrease computation time
            if (abs(num) < 0.2) { return this->conv; }
            else if (num.real() < 0)
            {
                if (abs(num) < 0.6) { return this->conv; }
            }
            std::complex<double> z(0.0, 0.0);                                   // start the 0-orbit
            int cap = 500;                                                      // set an iteration cap
            int i;
            for (i = 0; i < cap && abs(z) <= 2; i++)                            // iterate 0 on z_n+1 = z_n^2 + num
            {
                z = pow(z, 2) + num;
            }
            if (abs(z) <= 2) { return this->conv; }                             // if orbit has not diverged to infinity, return the background color
            else                                                               // otherwise, compute the scaled color
            {
                double div = cap/(double)i;
                int r = (int) 255*(this->r + (1/div)*(1-this->r));
                int g = (int) 255*(this->g + (1/div)*(1-this->g));
                int b = (int) 255*(this->b + (1/div)*(1-this->b));
                rgb_t color = { r, g, b };
                return color;
            }
        }
    };

    /* class that colors a complex number c by transforming it to another point in the
    * complex plane (via mapping to the Riemann sphere, rotating, and then mapping
    * back to the complex plane) and coloring according to regular mandelbrot coloring
    */
    class teardrop : public generator
    {
    private:

        mandelbrot mand;
        double theta;

        static stfd::vec3 to_riemann_sphere(std::complex<double> const& num)
        {
            double x = num.real();
            double y = num.imag();

            double denom = 1 + x * x + y * y;
            stfd::vec3 vec;
            vec.x = 2 * x / denom;
            vec.y = 2 * y / denom;
            vec.z = (-1 + x * x + y * y) / denom;
            return vec;
        }

        static std::complex<double> to_complex(stfd::vec3 const& vec)
        {
            if (vec.x == 0.0 && vec.y == 0.0 && vec.z == 1.0)
            {
                return std::complex<double>(DBL_MAX, DBL_MAX);
            }

            double a = vec.x / (1.0 - vec.z);
            double b = vec.y / (1.0 - vec.z);
            return std::complex<double>(a, b);
        }

    public:

        teardrop(double _theta, rgb_t conv, double r=1, double g=1, double b=1) : mand(mandelbrot(conv, r, g, b)), theta(_theta) {}

        rgb_t color_complex_num(std::complex<double> const& num) const
        {
            stfd::vec3 reimann_point = to_riemann_sphere(num);                      // map to riemann sphere
            double complement = stfd::constants::two_pi - theta;   // rotate by complement of theta since num is in the image space and we want the preimage
            stfd::vec3 rotated = stf::math::rotate(reimann_point, stfd::vec3(1, 0, 0), complement);
            std::complex<double> z = to_complex(rotated);                           // map back to the complex plane
            return mand.color_complex_num(z);
        }
    };

    /**
     * class that colors a complex number c according to the iterative rule z_n+1 = num^z_n where z_0 = num
     */
    class powertower : public generator {
    private:

        rgb_t conv;
        double r, g, b;

    public:

        // constructor
        powertower(rgb_t _conv, double _r=1, double _g=1, double _b=1)
            : conv(_conv)
            , r(_r)
            , g(_g)
            , b(_b)
        {}

        rgb_t color_complex_num(std::complex<double> const& num)const override
        {
            std::complex<double> z = num;                                       // start the input
            std::complex<double> prev = z;                                      // set up previous value
            int iter_cap = 200;                                                 // set an iteration cap
            double mag_cap = 50;                                                // set up a magnitude cap
            int i;
            for (i = 0; i < iter_cap && abs(z) < mag_cap; i++)                  // iterate 0 on z_n+1 = num^z_n
            {
                z = pow(num, z);                                                // exponentiate
            }
            if (abs(z) < mag_cap) { return this->conv; }                        // if orbit has not diverged to infinity, return the background color
            else                                                               // otherwise, compute the scaled color
            {
                double div = iter_cap/(double)i;
                div = 1000;
                int r = (int) 255*(this->r + (1/div)*(1-this->r));
                int g = (int) 255*(this->g + (1/div)*(1-this->g));
                int b = (int) 255*(this->b + (1/div)*(1-this->b));
                rgb_t color = { r, g, b };
                return color;
            }
        }

    };

    /**
     * class that colors a complex number c according to newton's method for finding zeros of a function
     * @todo turn this class to just take in the zeros
     */
    class newton : public generator
    {
    private:

        rgb_t div;
        std::complex<double> func(std::complex<double> z) const
        {
            return std::complex<double>(1, 0) * pow(z, 4)
                    + std::complex<double>(0, 0) * pow(z, 3)
                    + std::complex<double>(0, 0) * pow(z, 2)
                    + std::complex<double>(0, 0) * z
                    + std::complex<double>(-1, 0);
        }

        std::complex<double> deriv(std::complex<double> z) const
        {
            return std::complex<double>(4, 0) * pow(z, 3)
                    + std::complex<double>(0, 0) * pow(z, 2)
                    + std::complex<double>(0, 0) * z
                    + std::complex<double>(0, 0);
        }

        // zeros and colors
        std::complex<double> zeros [4] =
        {
            std::complex<double>(-1, 0),
            std::complex<double>(1, 0),
            std::complex<double>(0, -1),
            std::complex<double>(0, 1)
        };

        rgb_t colors [4] =
        {
            { 0, 5, 30 },
            { 0, 10, 170 },
            { 36, 70, 255 },
            { 0, 10, 70 }
        };

        std::complex<double> newtons_method(std::complex<double> num, double eps) const
        {
            std::complex<double> prev;
            int cap = 100;
            int i;
            for (i = 0; i < cap; i++) {
                prev = num;
                num = num - func(num) / deriv(num);
                if (abs(num - prev) <= eps) { return num; }
            }
            return num;
        }

        // method to return the index of the zeros array within eps (a small value)
        int index(std::complex<double> num, double eps) const
        {
            int index = -1;
            int length = sizeof(this->zeros) / sizeof(this->zeros[0]);
            int z;
            for (z = 0; z < length; z++)
            {
                if (abs(num - this->zeros[z]) <= eps) { index = z; }
            }
            return index;
        }

    public:

        // constructor
        newton(rgb_t _div) : div(_div) {}

        rgb_t color_complex_num(std::complex<double> const& num) const override
        {
            double eps = 0.000000001;
            std::complex<double> zero = newtons_method(num, eps);
            int index = this->index(zero, eps);
            if (index == -1) { return div; }
            else { return this->colors[index]; }
        }

    };

}