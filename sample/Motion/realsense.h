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

    // Color Buffer
    rs2::frame color_frame;
    cv::Mat color_mat;
    uint32_t color_width = 640;
    uint32_t color_height = 480;
    uint32_t color_fps = 30;

    // Gyro Buffer
    rs2::frame gyro_frame;
    rs2_vector gyro_data;
    uint32_t gyro_fps = 200;

    // Accel Buffer
    rs2::frame accel_frame;
    rs2_vector accel_data;
    uint32_t accel_fps = 63;

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

    // Update Gyro
    inline void updateGyro();

    // Update Accel
    inline void updateAccel();

    // Draw Data
    void draw();

    // Draw Color
    inline void drawColor();

    // Draw Gyro
    inline void drawGyro();

    // Draw Accel
    inline void drawAccel();

    // Show Data
    void show();

    // Show Color
    inline void showColor();

    // Show Gyro
    inline void showGyro();

    // Show Accel
    inline void showAccel();
};

#endif // __REALSENSE__