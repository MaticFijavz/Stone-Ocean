#pragma once

#include "StoneOcean_device.hpp"

//std
#include <string>
#include <vector>

namespace StoneOcean{

    struct pipelineConfigInfo{
        pipelineConfigInfo(const pipelineConfigInfo&) = delete;
        pipelineConfigInfo& operator=(const pipelineConfigInfo&) = delete;
        
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    
    class StoneOceanPipeline{
    public:
    StoneOceanPipeline(
     StoneOcean::StoneOceanDevice &device,
     const std::string& vertFilepath, 
     const std::string& fragFilepath, 
     const pipelineConfigInfo& configInfo);
     ~StoneOceanPipeline();
     
     StoneOceanPipeline(const StoneOceanPipeline&) = delete;
     StoneOceanPipeline& operator=(const StoneOceanPipeline&) = delete;
        
        void bind(VkCommandBuffer commandBuffer);
        
    static void defaultPipelineConfigInfo(pipelineConfigInfo& configInfo);
    
    private: 
        static std::vector<char> readFile(const std::string& filepath);
        
        void createGraphicsPipeline(const std::string& vertFilepath,
        const std::string& fragFilepath,      
        const pipelineConfigInfo& configInfo);
        
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const;
        
        StoneOcean::StoneOceanDevice& SOdevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
};
}
