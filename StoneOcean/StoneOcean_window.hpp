#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace  StoneOcean{

    class StoneOceanWindow{
    public:
        StoneOceanWindow(uint32_t w, uint32_t h, std::string name);
        ~StoneOceanWindow();
     
        StoneOceanWindow(const StoneOceanWindow &) = delete;
        StoneOceanWindow &operator=(const StoneOceanWindow &) = delete;
          
        bool ShouldClose() {return glfwWindowShouldClose(window);}

     
    private:
        void initWindow();
     
        const uint32_t width;
        const uint32_t height;
     
        std::string windowName;
        GLFWwindow *window;
    };
}
