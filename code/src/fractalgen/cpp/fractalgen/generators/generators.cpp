#include "fractalgen/generators/generators.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

namespace fractalgen::generators
{

    static constexpr int c_thread_count = 16;

    static constexpr int c_supersample_sqrt = 8;
    static constexpr double c_inset = 1.0 / (c_supersample_sqrt + 1);

    static void color_pixels(generator const& gen, window_t const& window, int min_j, int max_j, std::vector<rgb_t>& pixels, std::vector<bool>& status)
    {
        for (int j = min_j; j < max_j; ++j)
        {
            int offset = j * window.width;
            for (int i = 0; i < window.width; ++i)
            {
                pixels[offset + i] = gen.color_pixel(window, i, j);
                status[offset + i] = true;
            }
        }
    }

    static void color_pixels_ptr(generators::generator const* gen, window_t const& window, int min_j, int max_j, std::vector<rgb_t>* pixels, std::vector<bool>* status)
    {
        color_pixels(*gen, window, min_j, max_j, *pixels, *status);
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

    generator::generator(double phi) : m_phi(phi) {}

    std::vector<rgb_t> generator::generate(window_t const& window) const
    {
        std::vector<bool> status(window.width * window.height, false);

        time_t start = std::time(NULL);                                             // get start time

        std::vector<rgb_t> pixels;
        pixels.resize(window.width * window.height);

        // kick off threads
        std::vector<std::thread> threads;
        for (int t = 0; t < c_thread_count; ++t)
        {
            int min = (int)(t/(double)c_thread_count * window.height);
            int max = (int)((t+1)/(double)c_thread_count * window.height);
            threads.push_back(std::thread(color_pixels_ptr, this, window, min, max, &pixels, &status));
        }

        while (true)
        {
            size_t completed = 0;
            std::for_each(status.begin(), status.end(), [&completed](bool status) { if (status) { ++completed; } });
            double progress = static_cast<double>(completed) / (window.width * window.height);

            std::ostringstream stream;

            int bar_width = 50;

            {
                stream << "\rRendering " << name();
            }

            // write progress bar
            {
                stream << " [";
                int pos = bar_width * progress;
                for (int i = 0; i < bar_width; ++i)
                {
                    if (i <= pos) { stream << "#"; }
                    else { stream << " "; }
                }
                stream << "] ";
            }

            // add percentage
            {
                stream << std::fixed << std::setprecision(0) << (progress * 100.0) << "%";
            }

            // add duration
            {
                time_t current = std::time(NULL);
                stream << " -- " << current - start << " seconds elapsed";
            }

            std::cout << stream.str();
            std::cout.flush();

            if (completed == window.width * window.height)
            {
                break;
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        std::cout << std::endl;

        // join threads
        for (unsigned int t = 0; t < c_thread_count; ++t) { threads[t].join(); }
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
                if (m_phi != stfd::constants::zero)
                {
                    stfd::vec3 reimann_point = to_riemann_sphere(z);                // map to riemann sphere
                    // rotate by complement of phi since z is in the image space and we want the preimage
                    double complement = stfd::constants::two_pi - m_phi;
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

    mandelbrot::mandelbrot(double phi, rgb_t color, rgb_t diverging)
        : generator(phi), m_color(color), m_diverging()
    {
        m_diverging.x = static_cast<double>(diverging.r) / 255;
        m_diverging.y = static_cast<double>(diverging.g) / 255;
        m_diverging.z = static_cast<double>(diverging.b) / 255;
    }

    rgb_t mandelbrot::color_complex_num(std::complex<double> const& num) const
    {
        // quick check to decrease computation time
        if (abs(num) < 0.2) { return m_color; }
        else if (num.real() < 0)
        {
            if (abs(num) < 0.6) { return m_color; }
        }
        std::complex<double> z(0.0, 0.0);                                   // start the 0-orbit
        int cap = 500;                                                      // set an iteration cap
        int i;
        for (i = 0; i < cap && abs(z) <= 2; i++)                            // iterate 0 on z_n+1 = z_n^2 + num
        {
            z = pow(z, 2) + num;
        }
        if (abs(z) <= 2) { return m_color; }                                // if orbit has not diverged to infinity, return the background color
        else                                                                // otherwise, compute the scaled color
        {
            double scale = static_cast<double>(i) / cap;
            stfd::vec3 rgb = m_diverging + scale * (stfd::vec3(1) - m_diverging);
            stfi::vec3 bytes = (255.0 * rgb).as<int>();
            return { bytes.x, bytes.y, bytes.z };
        }
    }

    powertower::powertower(double phi, rgb_t color, rgb_t diverging)
        : generator(phi), m_color(color), m_diverging()
    {
        m_diverging.x = static_cast<double>(diverging.r) / 255;
        m_diverging.y = static_cast<double>(diverging.g) / 255;
        m_diverging.z = static_cast<double>(diverging.b) / 255;
    }

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
        if (abs(z) < mag_cap) { return m_color; }                           // if orbit has not diverged to infinity, return the background color
        else                                                                // otherwise, compute the scaled color
        {
            double div = iter_cap/(double)i;
            div = 1000;
            stfd::vec3 rgb = m_diverging + (1 / div) * (stfd::vec3(1) - m_diverging);
            stfi::vec3 bytes = (255.0 * rgb).as<int>();
            return { bytes.x, bytes.y, bytes.z };
        }
    }

    std::complex<double> newton::function::evaluate(std::complex<double> const& z) const
    {
        return scale * evaluate(roots.begin(), roots.end(), z);
    }

    std::complex<double> newton::function::evaluate_deriv(std::complex<double> const& z) const
    {
        return scale * evaluate_deriv(roots.begin(), roots.end(), z);
    }

    std::complex<double> newton::function::evaluate(iter begin, iter end, std::complex<double> const& z)
    {
        std::complex<double> res = 1.0;
        for (auto it = begin; it != end; ++it)
        {
            res *= ( z - it->z);
        }
        return res;
    }

    std::complex<double> newton::function::evaluate_deriv(iter begin, iter end, std::complex<double> const& z)
    {
        auto diff = end - begin;
        if (diff == 1)
        {
            return 1.0;
        }
        else
        {
            return evaluate(begin + 1, end, z) + (z - begin->z) * evaluate_deriv(begin + 1, end, z);
        }
    }

    std::complex<double> newton::newtons_method(std::complex<double> const& initial, double eps) const
    {
        std::complex<double> prev;
        int cap = 100;
        int i;
        std::complex<double> z = initial;
        for (i = 0; i < cap; i++) {
            prev = z;
            z = z - m_function.evaluate(z) / m_function.evaluate_deriv(z);
            if (abs(z - prev) <= eps) { return z; }
        }
        return z;
    }

    // method to return the index of the zeros array within eps (a small value)
    int newton::index(std::complex<double> const& z, double eps) const
    {
        std::vector<root> const& roots = m_function.roots;
        auto found = std::find_if(roots.begin(), roots.end(), [&](root const& r)
        {
            return std::abs(z - r.z) <= eps;
        });

        if (found != roots.end())
        {
            return found - roots.begin();
        }
        else
        {
            return -1;
        }
    }

    newton::newton(double phi, rgb_t diverging, std::vector<root> const& roots, std::complex<double> const& scale)
        : generator(phi),
        m_diverging(diverging),
        m_function({ roots, scale })
    {}

    rgb_t newton::color_complex_num(std::complex<double> const& num) const
    {
        double eps = 0.000000001;
        std::complex<double> zero = newtons_method(num, eps);
        int i = index(zero, eps);
        if (i == -1) { return m_diverging; }
        else { return m_function.roots[i].color; }
    }

}