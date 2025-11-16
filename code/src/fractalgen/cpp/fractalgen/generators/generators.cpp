#include "fractalgen/generators/generators.hpp"

#include <iostream>
#include <thread>

namespace fractalgen::generators
{

    static constexpr int c_thread_count = 16;

    static constexpr int c_supersample_sqrt = 8;
    static constexpr double c_inset = 1.0 / (c_supersample_sqrt + 1);

    static void color_pixels(generator const& gen, window_t const& window, int min_j, int max_j, std::vector<rgb_t>& pixels)
    {
        for (int j = min_j; j < max_j; ++j)
        {
            int offset = j * window.width;
            for (int i = 0; i < window.width; ++i)
            {
                pixels[offset + i] = gen.color_pixel(window, i, j);
            }
        }
    }

    static void color_pixels_ptr(generators::generator const* gen, window_t const& window, int min_j, int max_j, std::vector<rgb_t>* pixels)
    {
        color_pixels(*gen, window, min_j, max_j, *pixels);
    }

    window_t::window_t(stfd::aabb2 const& _bounds, int _width)
        : bounds(_bounds)
        , width(_width)
        , height(static_cast<int>(width * (bounds.diagonal().y / bounds.diagonal().x)))
        , delta_x(bounds.diagonal().x / width)
        , delta_y(bounds.diagonal().y / height)
        , inset_x(c_inset * delta_x)
        , inset_y(c_inset * delta_y)
    {}

    generator::generator(double rho) : m_rho(rho) {}

    std::vector<rgb_t> generator::generate(window_t const& window) const
    {
        std::cout << "Rendering " << name() << std::endl;
        time_t start = std::time(NULL);                                             // get start time

        std::vector<rgb_t> pixels;
        pixels.resize(window.width * window.height);

        // kick off threads
        std::vector<std::thread> threads;
        for (int t = 0; t < c_thread_count; ++t)
        {
            int min = (int)(t/(double)c_thread_count * window.height);
            int max = (int)((t+1)/(double)c_thread_count * window.height);
            threads.push_back(std::thread(color_pixels_ptr, this, window, min, max, &pixels));
        }

        // join threads
        for (unsigned int t = 0; t < c_thread_count; ++t) { threads[t].join(); }

        time_t current = std::time(NULL);                                           // get current time
        std::cout << "Completed " << name() << " -- " << current - start << " seconds elapsed" << std::endl;

        return pixels;
    }

    rgb_t generator::color_pixel(window_t const& window, int i, int j) const
    {
        int r = 0;
        int g = 0;
        int b = 0;
        double intial_x = window.bounds.min.x + i * window.delta_x + window.inset_x;
        double intial_y = window.bounds.min.y + j * window.delta_y + window.inset_y;
        for (size_t u = 0; u < c_supersample_sqrt; ++u)
        {
            for (size_t v = 0; v < c_supersample_sqrt; ++v)
            {
                double x = intial_x + u * window.inset_x;
                double y = intial_y + v * window.inset_y;
                std::complex<double> z(x, y);
                if (m_rho != stfd::constants::zero)
                {
                    stfd::vec3 reimann_point = to_riemann_sphere(z);                // map to riemann sphere
                    // rotate by complement of rho since z is in the image space and we want the preimage
                    double complement = stfd::constants::two_pi - m_rho;
                    stfd::vec3 rotated = stf::math::rotate(reimann_point, stfd::vec3(0, 1, 0), complement);
                    z = to_complex(rotated);                                        // map back to the complex plane
                }
                rgb_t color = color_complex_num(z);                         // virtual function call
                r += color.r;
                g += color.g;
                b += color.b;
            }
        }
        r /= (c_supersample_sqrt * c_supersample_sqrt);
        g /= (c_supersample_sqrt * c_supersample_sqrt);
        b /= (c_supersample_sqrt * c_supersample_sqrt);
        return { r, g, b };
    }

    stfd::vec3 generator::to_riemann_sphere(std::complex<double> const& num)
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

    std::complex<double> generator::to_complex(stfd::vec3 const& vec)
    {
        if (vec.x == 0.0 && vec.y == 0.0 && vec.z == 1.0)
        {
            return std::complex<double>(DBL_MAX, DBL_MAX);
        }

        double a = vec.x / (1.0 - vec.z);
        double b = vec.y / (1.0 - vec.z);
        return std::complex<double>(a, b);
    }

    mandelbrot::mandelbrot(double rho, rgb_t _conv, double _r, double _g, double _b)
        : generator(rho), conv(_conv) , r(_r) , g(_g) , b(_b) {}

    rgb_t mandelbrot::color_complex_num(std::complex<double> const& num) const
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

    powertower::powertower(double rho, rgb_t _conv, double _r, double _g, double _b)
        : generator(rho), conv(_conv), r(_r), g(_g), b(_b){}

    rgb_t powertower::color_complex_num(std::complex<double> const& num) const
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

    std::complex<double> newton::newtons_method(std::complex<double> num, double eps) const
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
    int newton::index(std::complex<double> num, double eps) const
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

    newton::newton(double rho, rgb_t _div) : generator(rho), div(_div) {}

    rgb_t newton::color_complex_num(std::complex<double> const& num) const
    {
        double eps = 0.000000001;
        std::complex<double> zero = newtons_method(num, eps);
        int index = this->index(zero, eps);
        if (index == -1) { return div; }
        else { return this->colors[index]; }
    }

}