#ifndef BODY_H
#define BODY_H

#include <vector>
#include "point.h"

class Body {
private:
    std::vector<double> coordinates;
public:
    Body(int dimensions);
    void move(int axis);
    const std::vector<double>& getCoordinates() const;
    std::vector<double> calculateDistances(const std::vector<Point>& points) const;
    std::vector<double> triangulate(const std::vector<Point>& points, const std::vector<double>& distances) const;
    double calculateError(const std::vector<Point>& points) const;
};

#endif // BODY_H