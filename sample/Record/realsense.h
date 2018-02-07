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

    // Infrared Buffer
    rs2::frame infrared_frame;
    cv::Mat infrared_mat;
    uint32_t infrared_width = 640;
    uint32_t infrared_height = 480;
    uint32_t infrared_fps = 30;

    // File
    std::string file_name = "file.bag";

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

    // Update Color
    inline void updateColor();

    // Update Depth
    inline void updateDepth();

    // Update Infrared
    inline void updateInfrared();

    // Draw Data
    void draw();

    // Draw Color
    inline void drawColor();

    // Draw Depth
    inline void drawDepth();

    // Draw Infrared
    inline void drawInfrared();

    // Show Data
    void show();

    // Show Color
    inline void showColor();

    // Show Depth
    inline void showDepth();

    // Show Infrared
    inline void showInfrared();
};

#endif // __REALSENSE__