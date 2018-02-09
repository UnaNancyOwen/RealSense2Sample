#ifndef __MULTIREALSENSE__
#define __MULTIREALSENSE__

#include "realsense.h"

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <memory>

class MultiRealSense
{
private:
    // RealSense
    std::vector<std::unique_ptr<RealSense>> realsenses;

public:
    // Constructor
    MultiRealSense();

    // Destructor
    ~MultiRealSense();

    // Processing
    void run();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeSensor( const rs2::device& device );

    // Finalize
    void finalize();
};

#endif // __MULTIREALSENSE__