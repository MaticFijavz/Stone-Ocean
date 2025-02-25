#pragma once
#include <vector>
#include <string>


namespace StoneOcean{
    
    class StoneOceanPipeLine{
    public: 
        StoneOceanPipeLine(const std::string vertfilepath, const std::string fragfilepath);
        

    private:
        static std::vector<char> readfile(const std::string& filepath);
        
        void CreateGraphicsPipeline(const std::string vertfilepath, const std::string fragfilepath);
    
    };

}
