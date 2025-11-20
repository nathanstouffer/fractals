#pragma once

#include <array>
#include <complex>
#include <memory>
#include <vector>

#include "fractalgen/generators/generators.hpp"

namespace fractalgen::generators
{

    enum class types
    {
        mandelbrot,
        powertower,
        newton,
    };

    // TODO (stouff) consider removing this struct and just using options
    struct config
    {
        types type;
        double rho;
        std::array<uint8_t, 3> color;
        std::array<uint8_t, 3> diverging;

        struct root
        {
            std::complex<double> z;
            std::array<uint8_t, 3> color;
        };

        std::vector<root> roots;
        std::complex<double> scale;

        config(types _type, double _rho) : type(_type), rho(_rho) {}
    };

    std::unique_ptr<generator> factory(config const& conf);

}