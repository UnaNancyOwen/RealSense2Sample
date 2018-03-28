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

    // Enable Advanced Mode
    enableAdvancedMode();
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_DEPTH, depth_width, depth_height, rs2_format::RS2_FORMAT_Z16, depth_fps );

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
}

// Enable Advanced Mode
inline void RealSense::enableAdvancedMode()
{
    // Retrieve Device
    rs2::device device = pipeline_profile.get_device();

    // Advanced Mode
    if( device.is<rs400::advanced_mode>() ){
        // Enable Advanced Mode
        rs400::advanced_mode advanced_mode = device.as<rs400::advanced_mode>();
        if( !advanced_mode.is_enabled() ){
            advanced_mode.toggle_advanced_mode( true );
        }

        // e.g. Control Depth Table (Clamp Depth to Specified Range)
        STDepthTableControl depth_table_control = advanced_mode.get_depth_table();
        depth_table_control.depthClampMin = 1000; // Min Depth 1.0m
        depth_table_control.depthClampMax = 2000; // Max Depth 2.0m
        advanced_mode.set_depth_table( depth_table_control );
    }
}

// Finalize
void RealSense::finalize()
{
    // Close Windows
    cv::destroyAllWindows();

    // Disable Advanced Mode
    disableAdvancedMode();

    // Stop Pipline
    pipeline.stop();
}

// Disable Advanced Mode
inline void RealSense::disableAdvancedMode()
{
    // Retrieve Device
    rs2::device device = pipeline_profile.get_device();

    // Advanced Mode
    if( device.is<rs400::advanced_mode>() ){
        // Hardware Reset and Disable Advanced Mode
        rs400::advanced_mode advanced_mode = device.as<rs400::advanced_mode>();
        if( advanced_mode.is_enabled() ){
            advanced_mode.hardware_reset();
            advanced_mode.toggle_advanced_mode( false );
        }
    }
}

// Update Data
void RealSense::update()
{
    // Update Frame
    updateFrame();

    // Update Depth
    updateDepth();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Depth
inline void RealSense::updateDepth()
{
    // Retrieve Depth Frame
    depth_frame = frameset.get_depth_frame();

    // Retrive Frame Size
    depth_width = depth_frame.as<rs2::video_frame>().get_width();
    depth_height = depth_frame.as<rs2::video_frame>().get_height();
}

// Draw Data
void RealSense::draw()
{
    // Draw Depth
    drawDepth();
}

// Draw Depth
inline void RealSense::drawDepth()
{
    // Create cv::Mat form Depth Frame
    depth_mat = cv::Mat( depth_height, depth_width, CV_16SC1, const_cast<void*>( depth_frame.get_data() ) );
}

// Show Data
void RealSense::show()
{
    // Show Depth
    showDepth();
}

// Show Depth
inline void RealSense::showDepth()
{
    if( depth_mat.empty() ){
        return;
    }

    // Scaling
    cv::Mat scale_mat;
    depth_mat.convertTo( scale_mat, CV_8U, -255.0 / 10000.0, 255.0 ); // 0-10000 -> 255(white)-0(black)
    //depth_mat.convertTo( scale_mat, CV_8U, 255.0 / 10000.0, 0.0 ); // 0-10000 -> 0(black)-255(white)

    // Apply False Colour
    //cv::applyColorMap( scale_mat, scale_mat, cv::COLORMAP_BONE );

    // Show Depth Image
    cv::imshow( "Depth", scale_mat );
}