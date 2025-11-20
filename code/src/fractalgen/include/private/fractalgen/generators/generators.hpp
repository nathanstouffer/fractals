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

    /**
     * Interface that provides a function to color an element of the complex plane
     */
    class generator
    {
    public:

        generator(double _rho);
        virtual ~generator() = default;

        std::vector<rgb_t> generate(window_t const& window) const;

        rgb_t color_pixel(window_t const& window, int i, int j) const;

        virtual rgb_t color_complex_num(std::complex<double> const& num) const = 0;

        virtual std::string_view const name() const = 0;

    private:

        double m_rho;

        static stfd::vec3 to_riemann_sphere(std::complex<double> const& num);

        static std::complex<double> to_complex(stfd::vec3 const& vec);

    };

    /**
     * class that colors a complex number according to the iterative rule z_n+1 = (z_n)^2 + c
     */
    class mandelbrot : public generator
    {
    private:

        rgb_t m_color;
        stfd::vec3 m_diverging;

    public:

        mandelbrot(double rho, rgb_t color, rgb_t diverging);

        rgb_t color_complex_num(std::complex<double> const& num) const override;

        std::string_view const name() const override { return "mandelbrot"; }

    };

    /**
     * class that colors a complex number c according to the iterative rule z_n+1 = num^z_n where z_0 = num
     */
    class powertower : public generator
    {
    private:

        rgb_t m_color;
        stfd::vec3 m_diverging;

    public:

        powertower(double rho, rgb_t color, rgb_t diverging);

        rgb_t color_complex_num(std::complex<double> const& num)const override;

        std::string_view const name() const override { return "powertower"; }

    };

    /**
     * class that colors a complex number c according to newton's method for finding zeros of a function
     * @todo turn this class to just take in the zeros
     */
    class newton : public generator
    {
    public:

        struct root
        {
            std::complex<double> z;
            rgb_t color;
        };

    private:

        struct function
        {

            std::vector<root> roots;
            std::complex<double> scale;

            std::complex<double> evaluate(std::complex<double> const& z) const;
            std::complex<double> evaluate_deriv(std::complex<double> const& z) const;
        };

        std::complex<double> newtons_method(std::complex<double> const& z, double eps) const;

        // method to return the index of the zeros array within eps (a small value)
        int index(std::complex<double> const& z, double eps) const;

    public:

        newton(double rho, rgb_t diverging, std::vector<root> const& roots, std::complex<double> const& scale);

        rgb_t color_complex_num(std::complex<double> const& z) const override;

        std::string_view const name() const override { return "newton"; }

    private:

        rgb_t m_diverging;
        function m_function;

    };

}