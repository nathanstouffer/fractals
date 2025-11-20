#include "fractalgen/generators/factory.hpp"

#include "fractalgen/rgb.hpp"

namespace fractalgen::generators
{

    static rgb_t convert(std::array<uint8_t, 3> const& arr)
    {
        return { arr[0], arr[1], arr[2] };
    }

    static uint8_t convert(double x)
    {
        return static_cast<uint8_t>(std::clamp(x, 0.0, 255.0));
    }

    static std::vector<newton::root> convert(std::vector<config::root> const& input)
    {
        std::vector<newton::root> roots;
        roots.reserve(input.size());
        for (config::root const& r : input)
        {
            roots.push_back({ { r[0], r[1] }, { convert(r[2]), convert(r[3]), convert(r[4]) } });
        }
        return roots;
    }

    std::unique_ptr<generator> factory(config const& cfg)
    {
        switch (cfg.type)
        {
            case types::mandelbrot:
                return std::make_unique<mandelbrot>(cfg.phi, convert(cfg.color), convert(cfg.diverging));
                break;
            case types::powertower:
                return std::make_unique<powertower>(cfg.phi, convert(cfg.color), convert(cfg.diverging));
                break;
            case types::newton:
                return std::make_unique<newton>(cfg.phi, convert(cfg.diverging), convert(cfg.roots));
                break;
            default: return nullptr;
        }
        return nullptr;
    }

}