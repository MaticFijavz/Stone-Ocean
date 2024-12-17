﻿#pragma once

#include "StoneOcean_device.hpp"

//std
#include <string>
#include <vector>

namespace StoneOcean{

    struct pipelineConfigInfo{};
    
    class StoneOceanPipeline{
    public:
    StoneOceanPipeline(
     StoneOcean::StoneOceanDevice &device,
     const std::string& vertFilepath, 
     const std::string& fragFilepath, 
     const pipelineConfigInfo& configInfo);
     ~StoneOceanPipeline() {}
     
     StoneOceanPipeline(const StoneOceanPipeline&) = delete;
        void operator=(const StoneOceanPipeline&) = delete;
        
    static pipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    
    private: 
        static std::vector<char> readFile(const std::string& filepath);
        
        void createGraphicsPipeline(const std::string& vertFilepath,
        const std::string& fragFilepath,      
        const pipelineConfigInfo& configInfo);
        
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
        
        StoneOcean::StoneOceanDevice& SOdevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
};
}
