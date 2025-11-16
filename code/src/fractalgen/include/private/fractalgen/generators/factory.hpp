#pragma once

#include <memory>

#include "fractalgen/rgb.hpp"
#include "fractalgen/generators/generators.hpp"

namespace fractalgen::generators
{

    enum class types
    {
        mandelbrot,
        powertower,
        newton,
    };

    struct config
    {

        types type;

        rgb_t converging;
        rgb_t diverging;

        double rho;

    };

    std::unique_ptr<generator> factory(config const& conf);

}