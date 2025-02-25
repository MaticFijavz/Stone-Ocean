#include "app.hpp"
    
//std
#include <iomanip>
#include <iostream>
#include <stdexcept>

int main(){
    StoneOcean::app app{};
    
    try{
        app.run();
    }catch (const std::exception &e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}