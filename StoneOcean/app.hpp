#pragma once

#include "stone_ocean_window.hpp"
#include "stone_ocean_pipeline.hpp"

namespace StoneOcean{
    class app{
    public: 
        static constexpr int WIDTH {800};
        static constexpr int HEIGHT {600}; 
        
        void run();
    private:
        StoneOceanWindow window{WIDTH, HEIGHT, "SMV naloga DrHouse"};
        StoneOceanPipeLine DrHouseicsPipeline{"shaders/vert_shader.vert.spv", "shaders/frag_shader.frag.spv"};     
    };


}