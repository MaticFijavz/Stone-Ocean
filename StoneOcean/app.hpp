#pragma once

#include "stone_ocean_window.hpp"

namespace StoneOcean{
    class app{
    public: 
        static constexpr int WIDTH {800};
        static constexpr int HEIGHT {600}; 
        
        void run();
    private:
        StoneOceanWindow window{WIDTH, HEIGHT, "First App"};
        
    };


}