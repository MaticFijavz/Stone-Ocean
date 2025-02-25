#include "stone_ocean_pipeline.hpp"

//std
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace StoneOcean
{
    StoneOceanPipeLine::StoneOceanPipeLine(
        const std::string vertfilepath, const std::string fragfilepath){
        StoneOceanPipeLine::CreateGraphicsPipeline(vertfilepath, fragfilepath);
    }
    std::vector<char> StoneOceanPipeLine::readfile(const std::string& filepath){
        
        std::fstream file{filepath, std::ios::ate | std::ios::binary};
        
        if (!file.is_open()){
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        
        file.close();
        return buffer;
    }
    void StoneOceanPipeLine::CreateGraphicsPipeline(
    const std::string vertfilepath, const std::string fragfilepath) {
        std::cout << "Current working directory: " << std::filesystem::current_path() << '\n';
        std::cout << "Trying to load vertex shader from: " << std::filesystem::absolute(vertfilepath) << '\n';
        std::cout << "Trying to load fragment shader from: " << std::filesystem::absolute(fragfilepath) << '\n';
        // Check input parameters
        if (vertfilepath.empty() || fragfilepath.empty()) {
            printf("Error: Empty shader file paths\n");
            return;
        }

        try {
            auto vertCode = readfile(vertfilepath);
            auto fragCode = readfile(fragfilepath);
        
            // Check if shaders were loaded successfully
            if (vertCode.empty() || fragCode.empty()) {
                printf("Error: Empty shader code\n");
                return;
            }
        
            printf("Vertex shader size: %zu\nFragment shader size: %zu\n",
                   vertCode.size(), fragCode.size());
        }
        catch (const std::exception& e) {
            printf("Error: %s\n", e.what());
        }
    }
    }
