#ifndef IMAGE_H
#define IMAGE_H

#include <Eigen/Dense>
#include "line_segments.h"

class ImageProcessor
{
public:
    void BeierNeelyMorph(
        const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& source,
        const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& destination,
        const Image::OrientedLineSegmentPairs& olsp,
        float timeStep,
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& result) const;

    void warp(
        const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& source,
        const Image::OrientedLineSegmentPairs& olsp,
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& result) const;

    void CrossDissolve(
        const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& source,
        const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& destination,
        float timeStep,
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& result) const;

private:
    // Bilinear interpolation function for interpolating pixel values
    unsigned char bilinearInterpolate(
        const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& source,
        const Eigen::Vector2d& warpPos) const;
        
};

#endif // IMAGE_H
