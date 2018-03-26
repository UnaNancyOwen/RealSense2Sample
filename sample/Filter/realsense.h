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

    // Depth Buffer
    rs2::frame depth_frame;
    cv::Mat depth_mat;
    uint32_t depth_width = 640;
    uint32_t depth_height = 480;
    uint32_t depth_fps = 30;

    // Filter
    rs2::frame filtered_frame;
    cv::Mat filtered_mat;
    uint32_t filtered_width;
    uint32_t filtered_height;
    rs2::decimation_filter decimation_filter;
    rs2::spatial_filter spatial_filter;
    rs2::temporal_filter temporal_filter;

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

    // Initialize Filter
    inline void initializeFilter();

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Frame
    inline void updateFrame();

    // Update Depth
    inline void updateDepth();

    // Apply Filters
    inline void applyFilters();

    // Apply Decimation Filter
    inline rs2::frame applyDecimationFilter( const rs2::frame& frame );

    // Apply Spatial Filter
    inline rs2::frame applySpatialFilter( const rs2::frame& frame );

    // Apply Temporal Filter
    inline rs2::frame applyTemporalFilter( const rs2::frame& frame );

    // Draw Data
    void draw();

    // Draw Depth
    inline void drawDepth();

    // Draw Filtered Depth
    inline void drawFilter();

    // Show Data
    void show();

    // Show Depth
    inline void showDepth();

    // Show Filtered Depth
    inline void showFilter();
};

#endif // __REALSENSE__