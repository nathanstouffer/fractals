#include "fractalgen/generators/factory.hpp"

namespace fractalgen::generators
{

    std::unique_ptr<generator> factory(config_t const& config)
    {
        double r = static_cast<double>(config.diverging.r) / 255;
        double g = static_cast<double>(config.diverging.g) / 255;
        double b = static_cast<double>(config.diverging.b) / 255;
        switch (config.type)
        {
            case config_t::types_t::mandelbrot:
                return std::make_unique<mandelbrot>(config.converging, r, g, b);
                break;
            case config_t::types_t::rotated_mandelbrot:
                return std::make_unique<rotated_mandelbrot>(config.theta, config.converging, r, g, b);
                break;
            case config_t::types_t::powertower:
                return std::make_unique<powertower>(config.converging, r, g, b);
                break;
            case config_t::types_t::newton:
                return std::make_unique<newton>(config.diverging);
                break;
            default: return nullptr;
        }
        return nullptr;
    }

}