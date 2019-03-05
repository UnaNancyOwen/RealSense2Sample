#include "realsense.h"

#include <vector>

// Constructor
RealSense::RealSense()
{
    // Initialize
    initialize();
}

// Destructor
RealSense::~RealSense()
{
    // Finalize
    finalize();
}

// Processing
void RealSense::run()
{
    // Main Loop
    while( !viewer.wasStopped() ){
        // Update Data
        update();

        // Draw Data
        draw();

        // Show Data
        show();

        // Key Check
        const int32_t key = cv::waitKey( 10 );
        if( key == 'q' ){
            break;
        }
    }
}

// Initialize
void RealSense::initialize()
{
    cv::setUseOptimized( true );

    // Initialize Sensor
    initializeSensor();

    // Initialize Pose
    initializePose();
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_POSE, rs2_format::RS2_FORMAT_6DOF );

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
}

// Initialize Pose
inline void RealSense::initializePose()
{
    // Create Window
    viewer = cv::viz::Viz3d( "Pose" );

    // Show Coordinate System
    //viewer.showWidget( "CoordinateSystem", cv::viz::WCameraPosition::WCameraPosition( 0.5 ) );

    // Show Grid Plane
    const cv::Point3d center     = cv::Point3d( 0.0, 0.0, 0.0 );
    const cv::Vec3d   normal     = cv::Vec3d( 0.0, 1.0, 0.0 );
    const cv::Vec3d   y_axis     = cv::Vec3d( 0.0, 0.0, 1.0 );
    const cv::Vec2i   resolution = cv::Vec2i( 100, 100 );
    const cv::Vec2d   interval   = cv::Vec2d( 0.1, 0.1 );
    cv::viz::WGrid grid( center, normal, y_axis, resolution, interval, cv::viz::Color::white() );
    viewer.showWidget( "Grid", grid );

    // Create Positon Hisutory
    constexpr int32_t history_size = 30;
    position_history = circular_buffer<cv::Vec3d>( history_size );
}

// Finalize
void RealSense::finalize()
{
    // Close Windows
    cv::destroyAllWindows();

    // Stop Pipline
    pipeline.stop();
}

// Update Data
void RealSense::update()
{
    // Update Frame
    updateFrame();

    // Update Pose
    updatePose();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Pose
inline void RealSense::updatePose()
{
    // Retrieve Pose Frame
    pose_frame = frameset.first_or_default( rs2_stream::RS2_STREAM_POSE );
}

// Draw Data
void RealSense::draw()
{
    // Draw Pose
    drawPose();
}

// Draw Pose
inline void RealSense::drawPose()
{
    // Retrieve Pose from Pose Frame
    pose = pose_frame.as<rs2::pose_frame>().get_pose_data();
}

// Show Data
void RealSense::show()
{
    // Show Pose
    showPose();
}

// Show Pose
inline void RealSense::showPose()
{
    // Retrieve Translation
    const cv::Vec3d translation = cv::Vec3d( pose.translation.x, pose.translation.y, pose.translation.z ) * 100.0;
    std::cout << "T = ( " << translation[0] << ", " << translation[1] << ", " << translation[2] << " )\n";

    // Retrieve Rotation
    cv::Vec3d rotation = cv::Vec3d( pose.rotation.x, pose.rotation.y, pose.rotation.z );
    std::cout << "R = ( " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << " )\n";

    // Update Position History
    position_history.push_front( translation );

    // Create Poly Line from Position History
    std::vector<cv::Vec3d> positions = { position_history.begin(), position_history.end() };
    cv::Mat poly_line( 1, static_cast<int32_t>( positions.size() ), CV_64FC3, reinterpret_cast<void*>( &positions[0] ) );

    // Show Pose
    viewer.showWidget( "CameraPose", cv::viz::WCameraPosition( 0.5 ), cv::Affine3d( rotation, translation ) );
    viewer.showWidget( "PoseHistory", cv::viz::WPolyLine( poly_line, cv::viz::Viz3d::Color::green() ) );
    viewer.spinOnce();
}