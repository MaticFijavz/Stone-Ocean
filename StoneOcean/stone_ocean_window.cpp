#include "stone_ocean_window.hpp"

namespace StoneOcean{

    StoneOceanWindow::StoneOceanWindow(int width, int height, std::string windowName) 
                        : width{width}, height{height}, windowName{windowName}{
        initWindow();
    }
    
    void StoneOceanWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
    StoneOceanWindow::~StoneOceanWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    }