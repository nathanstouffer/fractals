#include "fractalgen/generators/factory.hpp"

#include "fractalgen/rgb.hpp"

namespace fractalgen::generators
{

    static rgb_t convert(std::array<uint8_t, 3> const& arr)
    {
        return { arr[0], arr[1], arr[2] };
    }

    static std::vector<newton::root> convert(std::vector<config::root> const& input)
    {
        std::vector<newton::root> roots;
        roots.reserve(input.size());
        for (config::root const& r : input)
        {
            // TODO (stouff) figure out why this conversion isn't working
            roots.push_back({ r.z, convert(r.color) });
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
                return std::make_unique<newton>(cfg.phi, convert(cfg.diverging), convert(cfg.roots), cfg.scale);
                break;
            default: return nullptr;
        }
        return nullptr;
    }

}