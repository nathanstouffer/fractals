#pragma once

#include <memory>

#include "fractalgen/rgb.hpp"
#include "fractalgen/generators/generators.hpp"

namespace fractalgen::generators
{

    struct config_t
    {

        enum class types_t
        {
            mandelbrot,
            rotated_mandelbrot,
            powertower,
            newton,
        };

        types_t type;

        rgb_t converging;
        rgb_t diverging;

        double theta;   // used for rotated_mandelbrot

    };

    std::unique_ptr<generator> factory(config_t const& config);

}