#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>

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

    // Point Cloud Buffer
    rs2::pointcloud pointcloud;
    rs2::points points;
    cv::viz::Viz3d viewer;
    cv::Mat vertices_mat;
    cv::Mat texture_mat;

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

    // Initialize Point Cloud
    inline void initializePointCloud();

    // Keyboard Callback Function
    static void keyboardCallback( const cv::viz::KeyboardEvent& event, void* cookie );

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

    // Update Point Cloud
    inline void updatePointCloud();

    // Parallel

    // Draw Data
    void draw();

    // Draw Color
    inline void drawColor();

    // Draw Depth
    inline void drawDepth();

    // Draw Point Cloud
    inline void drawPointCloud();

    // Show Data
    void show();

    // Show Point Cloud
    inline void showPointCloud();
};

#endif // __REALSENSE__