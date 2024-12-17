#pragma once

#include "StoneOcean_window.hpp"
#include "StoneOcean_pipeline.hpp"

namespace StoneOcean{
    class App{
    
    public:
        static constexpr uint32_t WIDTH {800};
        static constexpr uint32_t HEIGHT {600};
            
        void run();
        
    private:
        StoneOceanWindow StoneOceanWindow{WIDTH, HEIGHT, "Stone Ocean"};
        StoneOceanPipeline StoneOceanPipeLine{"shaders/vert_shader.vert.spv", "shaders/frag_shader.frag.spv"};
        
    };

}