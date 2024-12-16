#include "StoneOcean_window.hpp"

#include <stdexcept>


namespace StoneOcean{

    StoneOceanWindow::StoneOceanWindow(uint32_t w, uint32_t h, std::string name) : width{w}, height{h}, windowName{name} { // Constructor
        initWindow();
    }
    
    StoneOceanWindow::~StoneOceanWindow(){
        glfwDestroyWindow(window); // Destroy window
        glfwTerminate(); // Terminate GLFW
    }
    
    void StoneOceanWindow::initWindow(){
        glfwInit(); // Initialize GLFW
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable window resizing
    
        window= glfwCreateWindow(width, height, windowName.c_str(),nullptr, nullptr); // Create window
    }
    void StoneOceanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    
    } 

}
