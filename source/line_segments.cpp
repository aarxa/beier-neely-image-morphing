#include "line_segments.h"
#include <cmath>
#include <iostream>

namespace Image {

// Define static members
const double OrientedLineSegment::A = 1.0; //A small
const double OrientedLineSegment::B = 2.0; //B 0.5~2.0
const double OrientedLineSegment::P = 0.5; //P 0.0~1.0

// Default constructor
OrientedLineSegment::OrientedLineSegment() {
    endPoints[0] = Eigen::Vector2d(0, 0);
    endPoints[1] = Eigen::Vector2d(1, 0);  // Default to a unit line segment
}

// Constructor with specified endpoints
OrientedLineSegment::OrientedLineSegment(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2) {
    endPoints[0] = p1;
    endPoints[1] = p2;
}

double OrientedLineSegment::getWeight(const Eigen::Vector2d& p) const {
    return std::pow((std::pow(length(), P) / (A + distance(p))), B);
}

double OrientedLineSegment::length() const {
    return (endPoints[1] - endPoints[0]).norm();
}

double OrientedLineSegment::distance(const Eigen::Vector2d& p) const {
    Eigen::Vector2d s = endPoints[0];
    Eigen::Vector2d t = endPoints[1];
    Eigen::Vector2d st = t - s;
    double u = (p - s).dot(st) / st.squaredNorm();
    if (u < 0.0) {
        return (p - s).norm();
    } else if (u > 1.0) {
        return (p - t).norm();
    } else {
        Eigen::Vector2d proj = s + u * st;
        return (p - proj).norm();
    }
}

Eigen::Vector2d OrientedLineSegment::perpendicular() const {
    Eigen::Vector2d st = endPoints[1] - endPoints[0];
    return Eigen::Vector2d(-st.y(), st.x()).normalized();
}

Eigen::Vector2d OrientedLineSegment::GetSourcePosition(
    const OrientedLineSegment& source, const OrientedLineSegment& destination, const Eigen::Vector2d& target) {
    
    Eigen::Vector2d s_dst = destination.endPoints[0];
    Eigen::Vector2d t_dst = destination.endPoints[1];
    Eigen::Vector2d perp_dst = destination.perpendicular();

    double u = (target - s_dst).dot(t_dst - s_dst) / (t_dst - s_dst).squaredNorm();
    double v = (target - s_dst).dot(perp_dst);

    Eigen::Vector2d s_src = source.endPoints[0];
    Eigen::Vector2d t_src = source.endPoints[1];
    Eigen::Vector2d perp_src = source.perpendicular();

    return s_src + u * (t_src - s_src) + v * perp_src;
}

OrientedLineSegment OrientedLineSegment::operator*(double s) const {
    return OrientedLineSegment(endPoints[0] * s, endPoints[1] * s);
}

OrientedLineSegment OrientedLineSegment::operator+(const OrientedLineSegment& ols) const {
    return OrientedLineSegment(endPoints[0] + ols.endPoints[0], endPoints[1] + ols.endPoints[1]);
}

// Output operator for OrientedLineSegment
std::ostream& operator<<(std::ostream& stream, const OrientedLineSegment& ols) {
    stream << ols.endPoints[0].transpose() << " " << ols.endPoints[1].transpose();
    return stream;
}

// Input operator for OrientedLineSegment
std::istream& operator>>(std::istream& stream, OrientedLineSegment& ols) {
    double x1, y1, x2, y2;
    // Extract values for the endpoints
    stream >> x1 >> y1 >> x2 >> y2;
    ols.endPoints[0] = Eigen::Vector2d(x1, y1);  // First endpoint
    ols.endPoints[1] = Eigen::Vector2d(x2, y2);  // Second endpoint
    return stream;
}

// OrientedLineSegmentPairs implementation
Eigen::Vector2d OrientedLineSegmentPairs::getSourcePosition(const Eigen::Vector2d& destination) const {
    Eigen::Vector2d totalDisplacement(0, 0);
    double totalWeight = 0.0;

    for (const auto& pair : *this) {
        Eigen::Vector2d displacement = OrientedLineSegment::GetSourcePosition(pair.first, pair.second, destination) - destination;
        double weight = pair.second.getWeight(destination);
        totalDisplacement += displacement * weight;
        totalWeight += weight;
    }

    return totalDisplacement / totalWeight;
}

// Output operator for OrientedLineSegmentPairs
std::ostream& operator<<(std::ostream& stream, const OrientedLineSegmentPairs& olsp) {
    stream << olsp.size() << std::endl;
    for (const auto& pair : olsp) {
        stream << pair.first << " " << pair.second << std::endl;
    }
    return stream;
}

// Input operator for OrientedLineSegmentPairs
std::istream& operator>>(std::istream& stream, OrientedLineSegmentPairs& olsp) {
    size_t size;
    if (!(stream >> size)) {
        throw std::runtime_error("Failed to read size of line segment pairs");
    }

    olsp.resize(size);
    for (size_t i = 0; i < size; i++) {
        if (!(stream >> olsp[i].first >> olsp[i].second)) {
            throw std::runtime_error("Failed to read oriented line segment pair");
        }
    }

    return stream;
}

}  // namespace Image
