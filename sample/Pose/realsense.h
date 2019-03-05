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

    // Pose Buffer
    rs2::frame pose_frame;
    rs2_pose pose;
    cv::viz::Viz3d viewer;

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

    // Initialize Pose
    inline void initializePose();

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Frame
    inline void updateFrame();

    // Update Pose
    inline void updatePose();

    // Draw Data
    void draw();

    // Draw Pose
    inline void drawPose();

    // Show Data
    void show();

    // Show Pose
    inline void showPose();
};

#endif // __REALSENSE__