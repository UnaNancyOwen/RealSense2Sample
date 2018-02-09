#include <iostream>
#include <sstream>

#include "multirealsense.h"

int main( int argc, char* argv[] )
{
    try{
        MultiRealSense multirealsense;
        multirealsense.run();
    } catch( std::exception& ex ){
        std::cout << ex.what() << std::endl;
    }

    return 0;
}