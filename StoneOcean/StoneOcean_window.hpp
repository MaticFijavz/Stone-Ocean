#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace  StoneOcean{

    class StoneOceanWindow{
    public:
        StoneOceanWindow(int w, int h, std::string name);
        ~StoneOceanWindow();
     
        StoneOceanWindow(const StoneOceanWindow &) = delete;
        StoneOceanWindow &operator=(const StoneOceanWindow &) = delete;
          
        bool ShouldClose() {return glfwWindowShouldClose(window);}
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }
        
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) const; 

     
    private:
        
        static void framebufferResizeCallBack(GLFWwindow *window, int width, int height);
        void initWindow();
     
        int width;
        int height;
        bool framebufferResized = false;
     
        std::string windowName;
        GLFWwindow *window;
    };
}
