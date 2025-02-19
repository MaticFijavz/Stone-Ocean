#include "StoneOcean_pipeline.hpp"
#include "StoneOcean_model.hpp"
#include "StoneOcean_swap_chain.hpp"


//std
#include <fstream>
#include <iostream>
#include <cassert>

VkExtent2D Smudge::StoneOceanSwapChain::extent;
std::array<VkAttachmentDescription, 2> Smudge::StoneOceanSwapChain::attachments;

namespace StoneOcean
{
    VkPipelineViewportStateCreateInfo  viewportInfo{};
    VkRect2D scissor{};
    VkViewport viewport{};
    StoneOceanPipeline::StoneOceanPipeline(
     StoneOceanDevice &device,
     const std::string& vertFilepath, 
     const std::string& fragFilepath, 
     const pipelineConfigInfo& configInfo) :  SOdevice{device} {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }
    StoneOceanPipeline::~StoneOceanPipeline(){
        vkDestroyShaderModule(SOdevice.device(), vertShaderModule, nullptr);
        vkDestroyShaderModule(SOdevice.device(), fragShaderModule, nullptr);
        vkDestroyPipeline(SOdevice.device(), graphicsPipeline, nullptr);
    }
    
    
    void StoneOceanPipeline::createGraphicsPipeline(
    const std::string& vertFilepath, 
    const std::string& fragFilepath, 
    const pipelineConfigInfo& configInfo
    )
    {
        assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipelineLayout provided");
        assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipelineLayout provided");
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);
        
        createShaderModule(vertCode, &vertShaderModule);
        createShaderModule(fragCode, &fragShaderModule);
        
        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;
        
        auto bindingDescriptions = StoneOceanmodel::Vertex::GetBindingDescriptions();
        auto attributeDescriptions = StoneOceanmodel::Vertex::GetAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
        dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
        dynamicStateInfo.flags = 0;
        
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(Smudge::StoneOceanSwapChain::extent.width);
        viewport.height = static_cast<float>(Smudge::StoneOceanSwapChain::extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        scissor.offset = {0, 0};
        scissor.extent = Smudge::StoneOceanSwapChain::extent;
  
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &viewport; 
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &scissor; 
        
        VkPipelineRasterizationStateCreateInfo rastainfo = configInfo.rasterizationInfo;
        rastainfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rastainfo.lineWidth = 1.0f;
        
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = configInfo.inputAssemblyInfo;
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // Use point list topology
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        
        std::vector<VkDynamicState> dynamic_states = configInfo.dynamicStateEnables;
                
        VkPipelineMultisampleStateCreateInfo multisampleInfo = configInfo.multisampleInfo;
        multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo = configInfo.depthStencilInfo;
        depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        
        // Define color blend attachment state
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE; // Disable blending
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        // Set up color blend state
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
        
        
        VkGraphicsPipelineCreateInfo pieplineInfo{};
        pieplineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pieplineInfo.stageCount = 2;
        pieplineInfo.pStages = shaderStages;
        pieplineInfo.pVertexInputState = &vertexInputInfo;
        pieplineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pieplineInfo.pViewportState = &viewportInfo;
        pieplineInfo.pRasterizationState = &rastainfo;
        pieplineInfo.pMultisampleState = &multisampleInfo;
        pieplineInfo.pColorBlendState = &colorBlending;
        pieplineInfo.pDepthStencilState = &depthStencilInfo;
        pieplineInfo.pDynamicState = &dynamicStateInfo;
         
        
        pieplineInfo.layout = configInfo.pipelineLayout;
        pieplineInfo.renderPass = configInfo.renderPass;
        pieplineInfo.subpass = configInfo.subpass;
        
        pieplineInfo.basePipelineIndex = -1;
        pieplineInfo.basePipelineHandle = VK_NULL_HANDLE;
        
        if (vkCreateGraphicsPipelines(SOdevice.device(), VK_NULL_HANDLE, 1, &pieplineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS){
            throw std::runtime_error("fuck you you didnt make a graphics pipeline ");
        }
    }
    
    
    std::vector<char> StoneOceanPipeline::readFile(const std::string& filepath){
    
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};    
    
        if (!file.is_open()){
            throw std::runtime_error("cannot open file: "+filepath);
        }
    
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char>buffer(fileSize);
    
        file.seekg(0);
        file.read(buffer.data(), fileSize);
    
        file.close();
        return buffer;
    }
    void StoneOceanPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(SOdevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
    }
    
        void StoneOceanPipeline::bind(VkCommandBuffer commandBuffer){
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        }

        
        void defaultPipelineConfigInfo(pipelineConfigInfo& configInfo){
        VkPipelineColorBlendStateCreateInfo colorBlendInfo = configInfo.colorBlendInfo;
        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO; 
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
      
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional
            
        configInfo.colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
         
        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = colorBlendInfo.pAttachments;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional
         
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional
        
        configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
        configInfo.dynamicStateInfo.dynamicStateCount =
            static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
        configInfo.dynamicStateInfo.flags = 0;
    }

}
