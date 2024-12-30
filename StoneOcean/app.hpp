#pragma once

#include "StoneOcean_window.hpp"
#include "StoneOcean_pipeline.hpp"
#include "StoneOcean_device.hpp"
#include "StoneOcean_swap_chain.hpp"
#include "StoneOcean_model.hpp"

//std
#include <memory>
#include <vector>


namespace StoneOcean{
    class App{
    public:
        static constexpr uint32_t WIDTH {800};
        static constexpr uint32_t HEIGHT {600};
            
        App();    
        ~App();    
        
        App(const StoneOceanWindow &) = delete;
        App &operator=(const StoneOceanWindow &) = delete;
        void run();
        
    private:
        void loadModels(); 
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
        
    
        StoneOceanWindow StoneOceanWindow{WIDTH, HEIGHT, "Stone Ocean"};
        StoneOceanDevice StoneOceanDevice{StoneOceanWindow};
        Smudge::StoneOceanSwapChain  StoneOceanSwapChain{StoneOceanDevice, StoneOceanWindow.getExtent()}; 
        std::unique_ptr<StoneOceanPipeline> StoneOceanPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<StoneOceanmodel> StoneOceanmodel;
        
    };

}