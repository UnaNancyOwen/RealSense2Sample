#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
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

    // Dispariry Buffer
    rs2::frame disparity_frame;
    cv::Mat disparity_mat;
    uint32_t disparity_width = depth_width;
    uint32_t disparity_height = depth_height;
    float baseline = 0.0f;

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

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Frame
    inline void updateFrame();

    // Update Depth
    inline void updateDepth();

    // Update Disparity
    inline void updateDisparity();

    // Draw Data
    void draw();

    // Draw Depth
    inline void drawDepth();

    // Draw Disparity
    inline void drawDisparity();

    // Show Data
    void show();

    // Show Depth
    inline void showDepth();

    // Show Disparity
    inline void showDisparity();
};

#endif // __REALSENSE__