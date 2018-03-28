#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
#include <librealsense2/rs_advanced_mode.hpp>
#include <opencv2/opencv.hpp>

class RealSense
{
private:
    // RealSense
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;

    // Depth Buffer
    rs2::frame depth_frame;
    cv::Mat depth_mat;
    uint32_t depth_width = 640;
    uint32_t depth_height = 480;
    uint32_t depth_fps = 30;

public:
    // Constructor
    RealSense();

    // Destructor
    ~RealSense();

    // Processing
    void run();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeSensor();

    // Enable Advanced Mode
    inline void enableAdvancedMode();

    // Finalize
    void finalize();

    // Disable Advanced Mode
    inline void disableAdvancedMode();

    // Update Data
    void update();

    // Update Frame
    inline void updateFrame();

    // Update Depth
    inline void updateDepth();

    // Draw Data
    void draw();

    // Draw Depth
    inline void drawDepth();

    // Show Data
    void show();

    // Show Depth
    inline void showDepth();
};

#endif // __REALSENSE__