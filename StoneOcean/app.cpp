#include "app.hpp"

#include <stdexcept>

namespace StoneOcean{
    
    App::App(){
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }
    App::~App(){
        vkDestroyPipelineLayout(StoneOceanDevice.device(), pipelineLayout, nullptr);
    }


    void App::run(){
        while(!StoneOceanWindow.ShouldClose()){
            glfwPollEvents();}
    }
    void StoneOcean::App::createPipelineLayout(){
        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(StoneOceanDevice.device(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }
    void StoneOcean::App::createPipeline(){
        auto pipelineConfig = StoneOceanPipeline::defaultPipelineConfigInfo(StoneOceanSwapChain.width() , StoneOceanSwapChain.height());
        pipelineConfig.renderPass = StoneOceanSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        StoneOceanPipeline = std::make_unique<class StoneOceanPipeline>(
            StoneOceanDevice,
            "shaders/vert_shader.vert.spv",
            "shaders/frag_shader.frag.spv",
            pipelineConfig);
    }
    void App::createCommandBuffers(){
    
    }
    void App::drawFrame(){
    
    }
}
