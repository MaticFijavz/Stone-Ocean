#include "StoneOcean_pipeline.hpp"


//std
#include <fstream>
#include <iostream>

namespace StoneOcean{
    
    StoneOceanPipeline::StoneOceanPipeline(const std::string& vertFilepath, const std::string& fragFilepath){
        createGraphicsPipeline(vertFilepath, fragFilepath);
    }
    
    void StoneOceanPipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath){
    
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);
    
        std::cout << "Vertex Shader Code: " << fragCode.size() << '\n';
        std::cout << "Vertex Shader Code: " << vertCode.size() <<'\n';
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

    
}
