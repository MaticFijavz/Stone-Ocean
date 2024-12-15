#include "app.hpp"

namespace StoneOcean{

    void App::run(){
        while(!StoneOceanWindow.ShouldClose()){
            glfwPollEvents();}
    }

}