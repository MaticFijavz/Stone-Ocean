#include "app.hpp"

#include <stdexcept>
#include <array>
#include <cassert>
#include <filesystem>

namespace StoneOcean{
    
    App::App(){
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
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
    
    void App::loadModels(){
        std::vector<StoneOceanmodel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        StoneOceanmodel= std::make_unique<class StoneOceanmodel>(StoneOceanDevice, vertices);
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
    
    void App::recreateSwapChain(){
        auto extent = StoneOceanWindow.getExtent();
        while (extent.width == 0 || extent.height == 0){
            extent = StoneOceanWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(StoneOceanDevice.device());
        
        if (StoneOceanSwapChain == nullptr) {
            StoneOceanSwapChain = std::make_unique<Smudge::StoneOceanSwapChain>(StoneOceanDevice, extent);
        }else{
            StoneOceanSwapChain = std::make_unique<Smudge::StoneOceanSwapChain>(StoneOceanDevice, extent, std::move(StoneOceanSwapChain));
            if (StoneOceanSwapChain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        StoneOceanSwapChain = nullptr;
        StoneOceanSwapChain = std::make_unique<Smudge::StoneOceanSwapChain>(StoneOceanDevice, extent);
        createPipeline();
    }
    
    void StoneOcean::App::createPipeline(){
        assert(StoneOceanSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");       
    
    
        pipelineConfigInfo pipelineConfig{};
        pipelineConfig.renderPass = StoneOceanSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        StoneOceanPipeline = std::make_unique<class StoneOceanPipeline>(
            StoneOceanDevice,
            "shaders/vert_shader.vert.spv",
            "shaders/frag_shader.frag.spv",
            pipelineConfig);
    }
    void App::createCommandBuffers(){
    
        commandBuffers.resize(StoneOceanSwapChain->imageCount());
        
        VkCommandBufferAllocateInfo allocInfo;
        allocInfo.sType= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = StoneOceanDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        allocInfo.pNext = nullptr;
        
        if (vkAllocateCommandBuffers(StoneOceanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
    
    void App::freeCommandBuffers(){
        vkFreeCommandBuffers(StoneOceanDevice.device(),
        StoneOceanDevice.getCommandPool(),
        static_cast<float>(commandBuffers.size()),
        commandBuffers.data());
        commandBuffers.clear();
    }

    
   void App::recordCommandBuffer(int imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        
    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
        throw std::runtime_error("failed to begin recording command buffer");
    }
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = StoneOceanSwapChain->getRenderPass();
    renderPassInfo.framebuffer = StoneOceanSwapChain->getFrameBuffer(imageIndex);
        
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = StoneOceanSwapChain->getSwapChainExtent();  
        
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.2f, 0.2f, 0.2f, 1.0f};
    clearValues[1].depthStencil.depth = 1.0f;
    clearValues[1].depthStencil.stencil = 0;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
        
    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(StoneOceanSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(StoneOceanSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, StoneOceanSwapChain->getSwapChainExtent()};
    
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);
    StoneOceanPipeline->bind(commandBuffers[imageIndex]);
    StoneOceanmodel->Bind(commandBuffers[imageIndex]);
    StoneOceanmodel->Draw(commandBuffers[imageIndex]);
    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
    
    void App::drawFrame(){
        uint32_t imageIndex;
        auto result = StoneOceanSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire next (swap chain) image");
        }

        recordCommandBuffer(imageIndex);
        result = StoneOceanSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || StoneOceanWindow.wasWindowResized()){
            StoneOceanWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS){
            throw std::runtime_error("failed to submit command buffer to swap chain image");
        }
    }
}
