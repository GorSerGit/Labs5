#ifndef POINT_H
#define POINT_H

#include <vector>
#include <cmath>

class Point {
private:
    std::vector<double> coordinates;
public:
    Point(const std::vector<double>& coords);
    const std::vector<double>& getCoordinates() const;
    double distanceTo(const Point& other) const;
};

#endif // POINT_H