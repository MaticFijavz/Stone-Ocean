#include "StoneOcean_window.hpp"

#include <stdexcept>


namespace StoneOcean{

    StoneOceanWindow::StoneOceanWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} { // Constructor
        initWindow();
    }
    
    StoneOceanWindow::~StoneOceanWindow(){
        glfwDestroyWindow(window); // Destroy window
        glfwTerminate(); // Terminate GLFW
    }
    
    void StoneOceanWindow::initWindow(){
        glfwInit(); // Initialize GLFW
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Disable window resizing
    
        window= glfwCreateWindow(width, height, windowName.c_str(),nullptr, nullptr); // Create window
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallBack);
    }
    void StoneOceanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) const
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    } 
    
    void StoneOceanWindow::framebufferResizeCallBack(GLFWwindow *window, int width, int height){
        auto SOwindow = reinterpret_cast<StoneOceanWindow *>(glfwGetWindowUserPointer(window));
        SOwindow->framebufferResized = true;
        SOwindow->width = width;
        SOwindow->height = height;
    }
}
