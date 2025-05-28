#include "point.h"

Point::Point(const std::vector<double>& coords) : coordinates(coords) {}

const std::vector<double>& Point::getCoordinates() const {
    return coordinates;
}

double Point::distanceTo(const Point& other) const {
    double sum = 0.0;
    for (int i = 0; i < coordinates.size(); ++i) {
        double diff = coordinates[i] - other.coordinates[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}