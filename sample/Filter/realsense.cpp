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

    // Initialize Filter
    initializeFilter();
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

// Initialize Filter
inline void RealSense::initializeFilter()
{
    // Set Decimation Filter Option
    if( decimation_filter.supports( rs2_option::RS2_OPTION_FILTER_MAGNITUDE ) ){
        rs2::option_range option_range = decimation_filter.get_option_range( rs2_option::RS2_OPTION_FILTER_MAGNITUDE );
        decimation_filter.set_option( rs2_option::RS2_OPTION_FILTER_MAGNITUDE, option_range.min ); // 1(min) is not downsampling
    }

    // Set Spatial Filter Option
    if( spatial_filter.supports( rs2_option::RS2_OPTION_HOLES_FILL ) ){
        rs2::option_range option_range = spatial_filter.get_option_range( rs2_option::RS2_OPTION_HOLES_FILL );
        spatial_filter.set_option( rs2_option::RS2_OPTION_HOLES_FILL, option_range.max ); // 5(max) is fill all holes
    }
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

    // Update Depth
    updateDepth();

    // Apply Filters
    applyFilters();
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

// Apply Filters
inline void RealSense::applyFilters()
{
    if( !depth_frame ){
        return;
    }

    filtered_frame = depth_frame;

    // Apply Decimation Filter (DownSampling)
    filtered_frame = applyDecimationFilter( filtered_frame );

    // Transform Disparity Frame from Depth Frame
    rs2::disparity_transform disparity_transform( true );
    filtered_frame = disparity_transform.process( filtered_frame );

    // Apply Spatial Filter (Edge-Preserving Smoothing, Hole Filling)
    filtered_frame = applySpatialFilter( filtered_frame );

    // Apply Temporal Filter (Smoothing using Several Previous Frames)
    filtered_frame = applyTemporalFilter( filtered_frame );

    // Transform Depth Frame from Disparity Frame
    rs2::disparity_transform depth_transform( false );
    filtered_frame = depth_transform.process( filtered_frame );

    // Retrive Frame Size
    filtered_width = filtered_frame.as<rs2::video_frame>().get_width();
    filtered_height = filtered_frame.as<rs2::video_frame>().get_height();
}

// Apply Decimation Filter
inline rs2::frame RealSense::applyDecimationFilter( const rs2::frame& frame )
{
    rs2::frame filtered_frame = decimation_filter.process( frame );
    return filtered_frame;
}

// Apply Spatial Filter
inline rs2::frame RealSense::applySpatialFilter( const rs2::frame& frame )
{
    rs2::frame filtered_frame = spatial_filter.process( frame );
    return filtered_frame;
}

// Apply Temporal Filter
inline rs2::frame RealSense::applyTemporalFilter( const rs2::frame& frame )
{
    rs2::frame filtered_frame = temporal_filter.process( frame );
    return filtered_frame;
}

// Draw Data
void RealSense::draw()
{
    // Draw Depth
    drawDepth();

    // Draw Filtered Depth
    drawFilter();
}

// Draw Depth
inline void RealSense::drawDepth()
{
    // Create cv::Mat form Depth Frame
    depth_mat = cv::Mat( depth_height, depth_width, CV_16SC1, const_cast<void*>( depth_frame.get_data() ) );
}

// Draw Filtered Depth
inline void RealSense::drawFilter()
{
    filtered_mat = cv::Mat( filtered_height, filtered_width, CV_16SC1, const_cast<void*>( filtered_frame.get_data() ) );
}

// Show Data
void RealSense::show()
{
    // Show Depth
    showDepth();

    // Show Filtered Depth
    showFilter();
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

// Show Filtered Depth
inline void RealSense::showFilter()
{
    if( filtered_mat.empty() ){
        return;
    }

    // Scaling
    cv::Mat scale_mat;
    filtered_mat.convertTo( scale_mat, CV_8U, -255.0 / 10000.0, 255.0 ); // 0-10000 -> 255(white)-0(black)
    //depth_mat.convertTo( scale_mat, CV_8U, 255.0 / 10000.0, 0.0 ); // 0-10000 -> 0(black)-255(white)

    // Apply False Colour
    //cv::applyColorMap( scale_mat, scale_mat, cv::COLORMAP_BONE );

    // Show filtered Depth Image
    cv::imshow( "Filtered Depth", scale_mat );
}