#pragma once

#include "StoneOcean_window.hpp"

namespace StoneOcean{
    class App{
    
    public:
        static constexpr uint32_t WIDTH {800};
        static constexpr uint32_t HEIGHT {600};
            
        void run();
        
    private:
        StoneOceanWindow StoneOceanWindow{WIDTH, HEIGHT, "Stone Ocean"};
    };

}