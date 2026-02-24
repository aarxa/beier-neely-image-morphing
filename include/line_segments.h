#ifndef LINE_SEGMENTS_INCLUDED
#define LINE_SEGMENTS_INCLUDED

#include <iostream>
#include <utility>
#include <vector>
#include <Eigen/Dense>

namespace Image
{
    /** This class represents an oriented line segment. */
    class OrientedLineSegment
    {
    public:
        static const double A;
        static const double B;
        static const double P;

        Eigen::Vector2d endPoints[2];

        // Default constructor
        OrientedLineSegment();

        // Constructor with two endpoints
        OrientedLineSegment(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2);

        double getWeight(const Eigen::Vector2d &p) const;
        double length() const;
        double distance(const Eigen::Vector2d &p) const;
        Eigen::Vector2d perpendicular() const;

        static Eigen::Vector2d GetSourcePosition(const OrientedLineSegment &source,
                                                 const OrientedLineSegment &destination,
                                                 const Eigen::Vector2d &target);

        OrientedLineSegment operator*(double s) const;
        OrientedLineSegment operator+(const OrientedLineSegment &ols) const;
    };

    std::ostream &operator<<(std::ostream &stream, const OrientedLineSegment &ols);
    std::istream &operator>>(std::istream &stream, OrientedLineSegment &ols);

    class OrientedLineSegmentPairs : public std::vector<std::pair<OrientedLineSegment, OrientedLineSegment>>
    {
    public:
        Eigen::Vector2d getSourcePosition(const Eigen::Vector2d &destination) const;
    };

    std::ostream &operator<<(std::ostream &stream, const OrientedLineSegmentPairs &olsp);
    std::istream &operator>>(std::istream &stream, OrientedLineSegmentPairs &olsp);
}
#endif // LINE_SEGMENTS_INCLUDED
