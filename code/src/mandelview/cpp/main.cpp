#include "mandelview/renderer.hpp"

#include <CLI/CLI.hpp>

namespace mandelview
{

    int main(int argc, char** argv)
    {
        CLI::App app{"mandelview is a tool the dynamically renders the mandelbrot set on the GPU.", "mandelview"};

        std::array<uint8_t, 3> color;
        app.add_option("-c,--color", color, "The color (0-255) assigned to diverging inputs. Format: R G B")
            ->type_name("R G B")
            ->capture_default_str();

        CLI11_PARSE(app, argc, argv);

        renderer r = renderer(color);
        int status = r.run();
        return status;
    }

}

int main(int argc, char** argv)
{
    return mandelview::main(argc, argv);
}