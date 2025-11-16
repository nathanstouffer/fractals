#pragma once

#include <array>
#include <memory>

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

        std::array<uint8_t, 3> color = { 0, 0, 0 };
        std::array<uint8_t, 3> diverging = { 0, 0, 0 };

        double rho;

    };

    std::unique_ptr<generator> factory(config const& conf);

}