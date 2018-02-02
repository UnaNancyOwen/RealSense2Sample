#include "realsense.h"

#include <vector>
#include <sstream>
#ifdef _OPENMP
#include <omp.h>
#endif

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

    // Initialize Point Cloud
    initializePointCloud();
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_COLOR, color_width, color_height, rs2_format::RS2_FORMAT_BGR8, color_fps );
    config.enable_stream( rs2_stream::RS2_STREAM_DEPTH, depth_width, depth_height, rs2_format::RS2_FORMAT_Z16, depth_fps );

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
}

// Initialize Point Cloud
inline void RealSense::initializePointCloud()
{
    // Create Window
    viewer = cv::viz::Viz3d( "Point Cloud" );

    // Register Keyboard Callback Function
    viewer.registerKeyboardCallback( &keyboardCallback, this );

    // Show Coordinate System
    viewer.showWidget( "CoordinateSystem", cv::viz::WCameraPosition::WCameraPosition( 0.5 ) );
}

// Keyboard Callback Function
void RealSense::keyboardCallback( const cv::viz::KeyboardEvent& event, void* cookie )
{
    // Exit Viewer when Pressed ESC key
    if( event.code == 'q' && event.action == cv::viz::KeyboardEvent::Action::KEY_DOWN ){

        // Retrieve Viewer
        cv::viz::Viz3d viewer = static_cast<RealSense*>( cookie )->viewer;

        // Close Viewer
        viewer.close();
    }
    // Save Point Cloud to File when Pressed 's' key
    else if( event.code == 's' && event.action == cv::viz::KeyboardEvent::Action::KEY_DOWN ){
        // Retrieve Point Cloud and Color
        cv::Mat cloud = static_cast<RealSense*>( cookie )->vertices_mat;
        cv::Mat color = static_cast<RealSense*>( cookie )->texture_mat;

        // Generate File Name
        static uint8_t i = 0;
        std::ostringstream oss;
        oss << std::setfill( '0' ) << std::setw( 3 ) << i++;
        std::string file = oss.str() + ".ply";

        // Write Point Cloud to File
        cv::viz::writeCloud( file, cloud, color, cv::noArray(), false );
    }
};

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

    // Update Color
    updateColor();

    // Update Depth
    updateDepth();

    // Update Point Cloud
    updatePointCloud();
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

// Update Depth
inline void RealSense::updateDepth()
{
    // Retrieve Depth Frame
    depth_frame = frameset.get_depth_frame();

    // Retrive Frame Size
    depth_width = depth_frame.as<rs2::video_frame>().get_width();
    depth_height = depth_frame.as<rs2::video_frame>().get_height();
}

// Update Point Cloud
inline void RealSense::updatePointCloud()
{
    // Calculate Point Cloud
    points = pointcloud.calculate( depth_frame );

    // Mapping Texture to Point Cloud
    pointcloud.map_to( color_frame );
}

// Draw Data
void RealSense::draw()
{
    // Draw Color
    drawColor();

    // Draw Depth
    drawDepth();

    // Draw Point Cloud
    drawPointCloud();
}

// Draw Color
inline void RealSense::drawColor()
{
    // Create cv::Mat form Color Frame
    color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.get_data() ) );
}

// Draw Depth
inline void RealSense::drawDepth()
{
    // Create cv::Mat form Depth Frame
    depth_mat = cv::Mat( depth_height, depth_width, CV_16SC1, const_cast<void*>( depth_frame.get_data() ) );
}

// Draw Point Cloud
inline void RealSense::drawPointCloud()
{
    // Retrieve Vetrices
    const rs2::vertex* vertices = points.get_vertices();

    // Retrieve Coordinated Texture
    const rs2::texture_coordinate* texture_coordinates = points.get_texture_coordinates();

    // Create cv::Mat from Vertices and Texture
    vertices_mat = cv::Mat( depth_height, depth_width, CV_32FC3, cv::Vec3f::all( std::numeric_limits<float>::quiet_NaN() ) );
    texture_mat = cv::Mat( depth_height, depth_width, CV_8UC3, cv::Vec3b::all( 0 ) );

    #pragma omp parallel for
    for( int32_t index = 0; index < points.size(); index++ ){
        if( vertices[index].z ){
            // Set Vetices to cv::Mat
            const rs2::vertex vertex = vertices[index];
            vertices_mat.at<cv::Vec3f>( index ) = cv::Vec3f( vertex.x, vertex.y, vertex.z );

            // Set Texture to cv::Mat
            const rs2::texture_coordinate texture_coordinate = texture_coordinates[index];
            const uint32_t x = static_cast<uint32_t>( texture_coordinate.u * static_cast<float>( color_width ) ); // [0.0, 1.0) -> [0, width)
            const uint32_t y = static_cast<uint32_t>( texture_coordinate.v * static_cast<float>( color_height ) ); // [0.0, 1.0) -> [0, height)
            if( ( 0 <= x ) && ( x < color_width ) && ( 0 <= y ) && ( y < color_height ) ){
                texture_mat.at<cv::Vec3b>( index ) = color_mat.at<cv::Vec3b>( y, x );
            }
        }
    }
}

// Show Data
void RealSense::show()
{
    // Show Point Cloud
    showPointCloud();
}

// Show Point Cloud
inline void RealSense::showPointCloud()
{
    if( vertices_mat.empty() ){
        return;
    }

    if( texture_mat.empty() ){
        return;
    }

    // Create Point Cloud
    cv::viz::WCloud cloud( vertices_mat, texture_mat );

    // Show Point Cloud
    viewer.showWidget( "Cloud", cloud );
    viewer.spinOnce();
}