#pragma once

#include "StoneOcean_device.hpp"
 
 // libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp" 
#include <vector>

namespace StoneOcean{
    class StoneOceanmodel{
        public:
            
            struct Vertex{
                glm::vec2 position;
                glm::vec3 color;
                
                static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
            };
        
            StoneOceanmodel(StoneOceanDevice &device, const std::vector<Vertex> &vertices);    
            ~StoneOceanmodel();    
            
            StoneOceanmodel(const StoneOceanmodel &) = delete;
            StoneOceanmodel &operator=(const StoneOceanmodel &) = delete;
            
            void Bind(VkCommandBuffer commandBuffer);
            void Draw(VkCommandBuffer commandBuffer);
    
        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);
        
            StoneOceanDevice &SOdevice;
            VkBuffer vertexBuffer;  
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;
    
    };
}