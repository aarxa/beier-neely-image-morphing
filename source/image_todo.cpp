#include "image.h"
#include "line_segments.h"
#include <Eigen/Dense>
#include <vector>

void ImageProcessor::BeierNeelyMorph(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &destination,
    const Image::OrientedLineSegmentPairs &olsp,
    float timeStep,
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &result) const
{
    // Students will implement the Beier-Neely Morphing algorithm here
    // Ensure timeStep is within the valid range
    timeStep = std::clamp(timeStep, 0.0f, 1.0f);

    int width = source.cols();
    int height = source.rows();
    result.resize(height, width); // Ensure result has the correct size

    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> warpedSource(height, width);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> warpedDestination(height, width);

    // Create two separate OLSP objects for warpedSource and warpedDestination at each timestep
    Image::OrientedLineSegmentPairs olspForWarpedSource;
    Image::OrientedLineSegmentPairs olspForWarpedDestination;
    
    // Interpolate feature lines at the current timestep and update both warped source and destination
    for (const auto& pair : olsp) {
        // Interpolate between source and destination line segments for each pair
        Image::OrientedLineSegment interpolated = pair.first * (1 - timeStep) + pair.second * timeStep;
        
        // Populate warped pairs: one for the source -> intermediate, and one for destination -> intermediate
        olspForWarpedSource.emplace_back(pair.first, interpolated); // Source -> Intermediate
        olspForWarpedDestination.emplace_back(pair.second, interpolated); // Destination -> Intermediate
   }
    
    // Warp source and destination images using respective olsp
    warp(source, olspForWarpedSource, warpedSource);
    warp(destination, olspForWarpedDestination, warpedDestination);

    // Cross-dissolve the warped images
    CrossDissolve(warpedSource, warpedDestination, timeStep, result);

}

void ImageProcessor::warp(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Image::OrientedLineSegmentPairs &olsp,
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &result) const
{
    // Students will implement the warp function here
     // Get the dimensions of the result image
    int height = result.rows();
    int width = result.cols();

    // Loop through each pixel in the result image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            // Create a 2D vector for the current pixel's location in the result image
            Eigen::Vector2d currentPixel(y, x);

            // Find the warped source pixel location based on the line segments
            Eigen::Vector2d warpedSourcePixel = olsp.getSourcePosition(currentPixel) + currentPixel;

            // Perform bilinear interpolation on the warped source pixel for this channel
            unsigned char interpolatedValue = bilinearInterpolate(source, warpedSourcePixel);

            // Set the result pixel's color value in the corresponding position
            result(y, x) = interpolatedValue;
        }
    }
}

unsigned char ImageProcessor::bilinearInterpolate(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Eigen::Vector2d &warpPos) const
{
    // Compute the integer positions
    int x1 = static_cast<int>(std::floor(warpPos.x()));
    int y1 = static_cast<int>(std::floor(warpPos.y()));
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    // Check if the coordinates are out of bounds and handle them
    if (x1 < 0 || x2 >= source.cols() || y1 < 0 || y2 >= source.rows()) {
        // Handle out-of-bounds by clamping to the nearest edge (repeating the edge pixel)
        x1 = std::max(0, std::min(x1, static_cast<int>(source.cols()) - 1));
        y1 = std::max(0, std::min(y1, static_cast<int>(source.rows()) - 1));
        x2 = std::max(0, std::min(x2, static_cast<int>(source.cols()) - 1));
        y2 = std::max(0, std::min(y2, static_cast<int>(source.rows()) - 1));
    }

    // Retrieve pixel values at the four corners of the bilinear interpolation
    unsigned char q11 = source(x1, y1);
    unsigned char q12 = source(x1, y2);
    unsigned char q21 = source(x2, y1);
    unsigned char q22 = source(x2, y2);

    // Compute the weights for interpolation (distance from the top-left corner)
    double dx = warpPos.x() - x1;
    double dy = warpPos.y() - y1;

    // Interpolate between the pixels horizontally
    double r1 = (1 - dx) * q11 + dx * q12;  // Interpolate horizontally for top row
    double r2 = (1 - dx) * q21 + dx * q22;  // Interpolate horizontally for bottom row

    // Interpolate vertically between the two horizontal results
    return static_cast<unsigned char>((1 - dy) * r1 + dy * r2);  // Final vertical interpolation

}

void ImageProcessor::CrossDissolve(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &destination,
    float timeStep,
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &result) const
{
    // Students will implement the Cross Dissolve function here
    // Get the dimensions of the source and destination images
    int height = source.rows();
    int width = source.cols();

    // Ensure that source and destination have the same dimensions
    if (source.rows() != destination.rows() || source.cols() != destination.cols()) {
        std::cerr << "Error: Source and destination images must have the same dimensions!" << std::endl;
        return;
    }

    // Loop over each pixel in the source and destination images
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Get the pixel values from source and destination
            unsigned char sourceValue = source(y, x);
            unsigned char destinationValue = destination(y, x);

            // Apply cross-dissolve to get the blended pixel value
            unsigned char blendedValue = static_cast<unsigned char>(
                (1 - timeStep) * sourceValue + timeStep * destinationValue
            );

            // Store the blended value in the result image
            result(y, x) = blendedValue;
        }
    }
}
