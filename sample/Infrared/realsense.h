#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include <array>

class RealSense
{
private:
    // RealSense
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;

    // Infrared Buffer
    std::array<rs2::frame, 2> infrared_frames;
    std::array<cv::Mat, 2> infrared_mats;
    uint32_t infrared_width = 640;
    uint32_t infrared_height = 480;
    uint32_t infrared_fps = 30;

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
    void updateFrame();

    // Update Infrared
    inline void updateInfrared();

    // Draw Data
    void draw();

    // Draw Infrared
    inline void drawInfrared();

    // Show Data
    void show();

    // Show Infrared
    inline void showInfrared();
};

#endif // __REALSENSE__