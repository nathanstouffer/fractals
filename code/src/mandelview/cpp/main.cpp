#include "mandelview/renderer.hpp"

namespace mandelview
{

    int main(int argc, char** argv)
    {
        renderer r = renderer({ 255, 0, 0 });
        int status = r.run();
        return status;
    }

}

int main(int argc, char** argv)
{
    return mandelview::main(argc, argv);
}