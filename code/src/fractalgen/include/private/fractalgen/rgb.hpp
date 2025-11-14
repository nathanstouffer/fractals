#pragma once

namespace fractalgen
{

    // TODO (stouff) probably replace this with stf::rgba
    struct rgb_t
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        rgb_t() = default;
        rgb_t(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
        rgb_t(int _r, int _g, int _b) : r(static_cast<unsigned char>(_r)), g(static_cast<unsigned char>(_g)), b(static_cast<unsigned char>(_b)) {}

    };

}