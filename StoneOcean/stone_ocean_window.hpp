#pragma once

#define  GLFW_INCLUDE_VULKAN 
#include <GLFW/glfw3.h>

#include <string>
namespace StoneOcean
{
    class StoneOceanWindow
    {
    public:
        StoneOceanWindow(int width, int height, std::string windowName);
        ~StoneOceanWindow();
        
        StoneOceanWindow(const StoneOceanWindow &) = delete;
        StoneOceanWindow &operator=(const StoneOceanWindow &) = delete;
        
        bool shouldClose() {return glfwWindowShouldClose(window);}
    
    private:
        void initWindow();
    
        const int width {};
        const int height {};
        std::string windowName {};
        GLFWwindow *window;
    }; 
}

