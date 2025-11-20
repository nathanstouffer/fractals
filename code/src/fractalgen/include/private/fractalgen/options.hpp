#pragma once

#include <array>
#include <string>

#include "fractalgen/generators/factory.hpp"
#include "fractalgen/generators/generators.hpp"

namespace fractalgen
{

    struct options final
    {

        struct mandelbrot_opts
        {
            std::array<uint8_t, 3> color = { 0, 0, 0 };
            std::array<uint8_t, 3> diverging = { 0, 100, 0 };

            void augment(generators::config& config) const
            {
                config.color = color;
                config.diverging = diverging;
            }
        };

        struct powertower_opts
        {
            std::array<uint8_t, 3> color = { 0, 0, 0 };
            std::array<uint8_t, 3> diverging = { 255, 255, 0 };

            void augment(generators::config& config) const
            {
                config.color = color;
                config.diverging = diverging;
            }
        };

        struct newton_opts
        {
            std::array<uint8_t, 3> diverging = { 0, 0, 0 };
            std::vector<generators::config::root> roots;
            std::complex<double> scale = 1.0;

            void augment(generators::config& config) const
            {
                config.diverging = diverging;
                config.roots = roots;
                config.scale = scale;
            }
        };

        generators::types type;
        std::string name = "fractal.png";
        std::array<double, 4> bounds = { -4, -1.5, 1.33, 1.5 };
        int width = 750;
        double phi = 0.0;

        mandelbrot_opts mandelbrot;
        powertower_opts powertower;
        newton_opts newton;

        generators::config config() const
        {
            generators::config cfg(type, phi);
            switch (type)
            {
                case generators::types::mandelbrot: mandelbrot.augment(cfg); break;
                case generators::types::powertower: powertower.augment(cfg); break;
                case generators::types::newton    : newton    .augment(cfg); break;
                default: break;
            }
            return cfg;
        }

        generators::window_t window() const
        {
            return { stfd::aabb2(stfd::vec2(bounds[0], bounds[1]), stfd::vec2(bounds[2], bounds[3])), width };
        }

    };

}