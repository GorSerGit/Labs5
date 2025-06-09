#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
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
    std::vector<std::pair<sf::Vector3f, sf::Vector3f>> gridLines_;
    
    void initializeGrid();
    sf::Vector2f projectPoint(const sf::Vector3f& point) const;
    void handleEvents(Body& body);
    void render(const std::vector<Point>& points, const Body& body);
    void drawLine(const sf::Vector3f& start, const sf::Vector3f& end, sf::Color color);
};

#endif // VISUALIZATION_H