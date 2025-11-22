#pragma once

#include <stf/stf.hpp>

namespace mandelview
{

    class renderer
    {
    public:

        renderer(std::array<uint8_t, 3> const& color);

        int run();

    private:

        stff::vec3 m_color;

    };

}