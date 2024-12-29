#include "app.hpp"

#include <stdexcept>
#include <array>
#include <filesystem>

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
            glfwPollEvents();
            drawFrame();
            }
            
        vkDeviceWaitIdle(StoneOceanDevice.device());
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
    
        commandBuffers.resize(StoneOceanSwapChain.imageCount());
        
        VkCommandBufferAllocateInfo allocInfo;
        allocInfo.sType= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = StoneOceanDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        allocInfo.pNext = nullptr;
        
        if (vkAllocateCommandBuffers(StoneOceanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers!");
        }
        
        for (uint32_t i = 0; i < commandBuffers.size(); i++){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
                throw std::runtime_error("failed to beign recording command buffer");
            }
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = StoneOceanSwapChain.getRenderPass();
            renderPassInfo.framebuffer = StoneOceanSwapChain.getFrameBuffer(i);
            
            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = StoneOceanSwapChain.getSwapChainExtent();  
            
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.2f, 0.2f, 0.2f, 1.0f};
            clearValues[1].depthStencil.depth = 1.0f;
            clearValues[1].depthStencil.stencil = 0;
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();
            
            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            
            StoneOceanPipeline->bind(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
            
            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
                throw std::runtime_error("failed to record command buffer");
            }
        }
        
    }
    void App::drawFrame(){
        uint32_t imageIndex;
        auto result = StoneOceanSwapChain.acquireNextImage(&imageIndex);
        
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire next (swap chain) image");
        }
        
        result = StoneOceanSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS){
            throw std::runtime_error("failed to submit command buffer to swap chain image");
        }
    
    }
}
