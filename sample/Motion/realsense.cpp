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
    // Initialize Sensor
    initializeSensor();
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_COLOR, color_width, color_height, rs2_format::RS2_FORMAT_BGR8, color_fps );
    config.enable_stream( rs2_stream::RS2_STREAM_GYRO, rs2_format::RS2_FORMAT_MOTION_XYZ32F, gyro_fps );
    config.enable_stream( rs2_stream::RS2_STREAM_ACCEL, rs2_format::RS2_FORMAT_MOTION_XYZ32F, accel_fps );

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
}

// Finalize
void RealSense::finalize()
{
    // Stop Pipline
    pipeline.stop();
}

// Update Data
void RealSense::update()
{
    // Update Frame
    updateFrame();

    // Update Color
    updateColor();

    // Update Gyro
    updateGyro();

    // Update Accel
    updateAccel();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Color
inline void RealSense::updateColor()
{
    // Retrieve Color Frame
    color_frame = frameset.get_color_frame();

    // Retrive Frame Size
    color_width = color_frame.as<rs2::video_frame>().get_width();
    color_height = color_frame.as<rs2::video_frame>().get_height();
}

// Update Gyro
inline void RealSense::updateGyro()
{
    // Retrieve Gyro Frame
    gyro_frame = frameset.first_or_default( rs2_stream::RS2_STREAM_GYRO );

    // Retrieve Gyro Data
    gyro_data = gyro_frame.as<rs2::motion_frame>().get_motion_data();
}

// Update Accel
inline void RealSense::updateAccel()
{
    // Retrieve Accel Frame
    accel_frame = frameset.first_or_default( rs2_stream::RS2_STREAM_ACCEL );

    // Retrieve Accel Data
    accel_data = accel_frame.as<rs2::motion_frame>().get_motion_data();
}

// Draw Data
void RealSense::draw()
{
    // Draw Color
    drawColor();

    // Draw Gyro
    drawGyro();

    // Draw Accel
    drawAccel();
}

// Draw Color
inline void RealSense::drawColor()
{
    // Create cv::Mat form Color Frame
    color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast< void* >( color_frame.get_data() ) );
}

// Draw Gyro
inline void RealSense::drawGyro()
{
    if( color_mat.empty() ){
        return;
    }

    // Draw Gyro
    std::ostringstream oss;
    oss << "Gyro : ( " << gyro_data.x << ", " << gyro_data.y << ", " << gyro_data.z << " )";
    cv::putText( color_mat, oss.str(), cv::Point( 20, 20 ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all( 255 ) );
}

// Draw Accel
inline void RealSense::drawAccel()
{
    if( color_mat.empty() ){
        return;
    }

    // Draw Accel
    std::ostringstream oss;
    oss << "Accel : ( " << accel_data.x << ", " << accel_data.y << ", " << accel_data.z << " )";
    cv::putText( color_mat, oss.str(), cv::Point( 20, 40 ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all( 255 ) );
}

// Show Data
void RealSense::show()
{
    // Show Color
    showColor();

    // Show Gyro
    showGyro();

    // Show Accel
    showAccel();
}

// Show Color
inline void RealSense::showColor()
{
    if( color_mat.empty() ){
        return;
    }

    // Show Color Image
    cv::imshow( "Motion", color_mat );
}

// Show Gyro
inline void RealSense::showGyro()
{
    // Show Gyro Data
    std::cout << "Gyro : (" << gyro_data.x << ", " << gyro_data.y << ", " << gyro_data.z << " )" << std::endl;
}

// Show Accel
inline void RealSense::showAccel()
{
    // Show Accel Data
    std::cout << "Accel : (" << accel_data.x << ", " << accel_data.y << ", " << accel_data.z << " )" << std::endl;
}
