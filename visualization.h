#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "point.h"
#include "body.h"

class Visualization {
public:
    Visualization();
    void run(const std::vector<Point>& points, Body& body);
    
private:
    sf::RenderWindow window_;
    const float CAMERA_DISTANCE = 300.0f;
    float angleX_ = 0.0f;
    float angleY_ = 0.0f;
    
    sf::Vector2f projectPoint(const sf::Vector3f& point) const;
    void handleEvents(Body& body);
    void render(const std::vector<Point>& points, const Body& body);
};

#endif // VISUALIZATION_H