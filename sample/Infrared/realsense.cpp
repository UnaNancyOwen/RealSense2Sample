#include "realsense.h"

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
    while( true ){
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
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_INFRARED, 1, infrared_width, infrared_height, rs2_format::RS2_FORMAT_Y8, infrared_fps ); // Left
    config.enable_stream( rs2_stream::RS2_STREAM_INFRARED, 2, infrared_width, infrared_height, rs2_format::RS2_FORMAT_Y8, infrared_fps ); // Right

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
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

    // Update Infrared
    updateInfrared();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Infrared
inline void RealSense::updateInfrared()
{
    // Retrieve Infrared Frame
    frameset.foreach( [this]( const rs2::frame& frame ){
        if( frame.get_profile().stream_type() == rs2_stream::RS2_STREAM_INFRARED ){
            infrared_frames[frame.get_profile().stream_index() - 1] = frame;
        }
    } );

    // Retrive Frame Size
    const rs2::frame& infrared_frame = infrared_frames.front();
    infrared_width = infrared_frame.as<rs2::video_frame>().get_width();
    infrared_height = infrared_frame.as<rs2::video_frame>().get_height();
}

// Draw Data
void RealSense::draw()
{
    // Draw Infrared
    drawInfrared();
}

// Draw Infrared
inline void RealSense::drawInfrared()
{
    // Create cv::Mat form Infrared Frame
    infrared_mats[0] = cv::Mat( infrared_height, infrared_width, CV_8UC1, const_cast<void*>( infrared_frames[0].get_data() ) ); // Left
    infrared_mats[1] = cv::Mat( infrared_height, infrared_width, CV_8UC1, const_cast<void*>( infrared_frames[1].get_data() ) ); // Right
}

// Show Data
void RealSense::show()
{
    // Show Infrared
    showInfrared();
}

// Show Infrared
inline void RealSense::showInfrared()
{
    if( infrared_mats.empty() ){
        return;
    }

    // Show Infrared Image
    cv::imshow( "Infrared - Left", infrared_mats[0] ); // Left
    cv::imshow( "Infrared - Right", infrared_mats[1] ); // Right
}