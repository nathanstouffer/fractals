#pragma once

#include <cfloat>
#include <cmath>

#include <complex>
#include <iostream>

#include <stf/math/transform.hpp>
#include <stf/stf.hpp>

#include "fractalgen/rgb.hpp"

namespace fractalgen::generators
{

    /**
     * Interface that provides a function to color an element of the complex plane
     */
    class generator
    {
    private:

        static constexpr int c_thread_count = 16;

        static constexpr int c_supersample_sqrt = 8;
        static constexpr double c_inset = 1.0 / (c_supersample_sqrt + 1);


    public:

        struct window_t
        {
            stfd::aabb2 bounds;
            int width;
            int height;

            double delta_x;
            double delta_y;

            double inset_x;
            double inset_y;

            window_t(stfd::aabb2 const& _bounds, int _width);

        };

    public:

        virtual ~generator() = default;

        std::vector<rgb_t> generate(window_t const& window) const;

        rgb_t color_pixel(window_t const& window, int i, int j) const;

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
        mandelbrot(rgb_t _conv, double _r=1, double _g=1, double _b=1);

        rgb_t color_complex_num(std::complex<double> const& num) const override;
    };

    /* class that colors a complex number c by transforming it to another point in the
    * complex plane (via mapping to the Riemann sphere, rotating, and then mapping
    * back to the complex plane) and coloring according to regular mandelbrot coloring
    */
    class rotated_mandelbrot : public generator
    {
    private:

        mandelbrot mand;
        double theta;

        static stfd::vec3 to_riemann_sphere(std::complex<double> const& num);

        static std::complex<double> to_complex(stfd::vec3 const& vec);

    public:

        rotated_mandelbrot(double _theta, rgb_t conv, double r=1, double g=1, double b=1);

        rgb_t color_complex_num(std::complex<double> const& num) const;
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
        powertower(rgb_t _conv, double _r=1, double _g=1, double _b=1);

        rgb_t color_complex_num(std::complex<double> const& num)const override;

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

        std::complex<double> newtons_method(std::complex<double> num, double eps) const;

        // method to return the index of the zeros array within eps (a small value)
        int index(std::complex<double> num, double eps) const;

    public:

        // constructor
        newton(rgb_t _div);

        rgb_t color_complex_num(std::complex<double> const& num) const override;

    };

}