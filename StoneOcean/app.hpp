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
        static constexpr uint32_t WIDTH {1600};
        static constexpr uint32_t HEIGHT {900};
            
        App();    
        ~App();    
        
        App(const StoneOceanWindow &) = delete; 
        App &operator=(const StoneOceanWindow &) = delete;
        void run();
        
    private:
        void loadModels(); 
        void createPipelineLayout();
        void createPipeline();
        void recordCommandBuffer(int imageIndex);
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        
    
        StoneOceanWindow StoneOceanWindow{WIDTH, HEIGHT, "Stone Ocean"};
        StoneOceanDevice StoneOceanDevice{StoneOceanWindow};
        std::unique_ptr<Smudge::StoneOceanSwapChain>StoneOceanSwapChain; 
        std::unique_ptr<StoneOceanPipeline> StoneOceanPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<StoneOceanmodel> StoneOceanmodel;
        
    };

}