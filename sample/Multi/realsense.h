#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include <string>

class RealSense
{
private:
    // RealSense
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;
    std::string serial_number;
    std::string friendly_name;

    // Color Buffer
    rs2::frame color_frame;
    cv::Mat color_mat;
    uint32_t color_width = 640;
    uint32_t color_height = 480;
    uint32_t color_fps = 30;

    // Depth Buffer
    rs2::frame depth_frame;
    cv::Mat depth_mat;
    uint32_t depth_width = 640;
    uint32_t depth_height = 480;
    uint32_t depth_fps = 30;

public:
    // Constructor
    RealSense( const std::string serial_number, const std::string friendly_name = "" );

    // Destructor
    ~RealSense();

    // Update Data
    void update();

    // Draw Data
    void draw();

    // Show Data
    void show();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeSensor();

    // Finalize
    void finalize();

    // Update Frame
    inline void updateFrame();

    // Update Color
    inline void updateColor();

    // Update Depth
    inline void updateDepth();

    // Draw Color
    inline void drawColor();

    // Draw Depth
    inline void drawDepth();

    // Show Color
    inline void showColor();

    // Show Depth
    inline void showDepth();
};

#endif // __REALSENSE__