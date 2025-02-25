#include "app.hpp"

namespace StoneOcean
{
    void app::run()
    {
        while (!window.shouldClose())
        {
            glfwPollEvents();
        }
    }

}
