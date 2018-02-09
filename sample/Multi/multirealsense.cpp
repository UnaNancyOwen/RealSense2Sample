#include "multirealsense.h"

// Constructor
MultiRealSense::MultiRealSense()
{
    // Initialize
    initialize();
}

// Destructor
MultiRealSense::~MultiRealSense()
{
    // Finalize
    finalize();
}

// Processing
void MultiRealSense::run()
{
    // Main Loop
    while( true ){
        for( std::unique_ptr<RealSense>& realsense : realsenses ){
            // Update Data
            realsense->update();

            // Draw Data
            realsense->draw();

            // Show Data
            realsense->show();
        }

        // Key Check
        const int32_t key = cv::waitKey( 10 );
        if( key == 'q' ){
            break;
        }
    }
}

// Initialize
void MultiRealSense::initialize()
{
    cv::setUseOptimized( true );

    // Retrive Connected Sensors List
    rs2::context context;
    const rs2::device_list device_list = context.query_devices();

    // Initialize Connected Sensors
    for( const rs2::device& device : device_list ){
        // Check Device
        // "Platform Camera" is not RealSense Devices
        const std::string friendly_name = device.get_info( rs2_camera_info::RS2_CAMERA_INFO_NAME );
        if( friendly_name == "Platform Camera" ){
            continue;
        }

        // Initialize Sensor
        initializeSensor( device );
    }
}

// Initialize Sensor
inline void MultiRealSense::initializeSensor( const rs2::device& device )
{
    // Retrive Serial Number (and Friendly Name)
    const std::string serial_number = device.get_info( rs2_camera_info::RS2_CAMERA_INFO_SERIAL_NUMBER );
    const std::string friendly_name = device.get_info( rs2_camera_info::RS2_CAMERA_INFO_NAME );

    // Add Sensor to Container
    realsenses.push_back( std::make_unique<RealSense>( serial_number, friendly_name ) );
}

// Finalize
void MultiRealSense::finalize()
{
    // Close Windows
    cv::destroyAllWindows();
}