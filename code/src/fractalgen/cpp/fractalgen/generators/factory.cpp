#include "fractalgen/generators/factory.hpp"

namespace fractalgen::generators
{

    std::unique_ptr<generator> factory(config const& conf)
    {
        double r = static_cast<double>(conf.diverging.r) / 255;
        double g = static_cast<double>(conf.diverging.g) / 255;
        double b = static_cast<double>(conf.diverging.b) / 255;
        switch (conf.type)
        {
            case types::mandelbrot:
                return std::make_unique<mandelbrot>(conf.converging, r, g, b);
                break;
            case types::rotated_mandelbrot:
                return std::make_unique<rotated_mandelbrot>(conf.theta, conf.converging, r, g, b);
                break;
            case types::powertower:
                return std::make_unique<powertower>(conf.converging, r, g, b);
                break;
            case types::newton:
                return std::make_unique<newton>(conf.diverging);
                break;
            default: return nullptr;
        }
        return nullptr;
    }

}