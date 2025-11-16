#include "fractalgen/generators/factory.hpp"

namespace fractalgen::generators
{

    std::unique_ptr<generator> factory(config const& cnfg)
    {
        double r = static_cast<double>(cnfg.diverging.r) / 255;
        double g = static_cast<double>(cnfg.diverging.g) / 255;
        double b = static_cast<double>(cnfg.diverging.b) / 255;
        switch (cnfg.type)
        {
            case types::mandelbrot:
                return std::make_unique<mandelbrot>(cnfg.theta, cnfg.converging, r, g, b);
                break;
            case types::powertower:
                return std::make_unique<powertower>(cnfg.theta, cnfg.converging, r, g, b);
                break;
            case types::newton:
                return std::make_unique<newton>(cnfg.theta, cnfg.diverging);
                break;
            default: return nullptr;
        }
        return nullptr;
    }

}