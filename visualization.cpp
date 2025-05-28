#include "visualization.h"
#include <iostream>
#include <cmath>

Visualization::Visualization() : window_(sf::VideoMode(800, 600), "3D Visualization") {
    window_.setFramerateLimit(60);
}

sf::Vector2f Visualization::projectPoint(const sf::Vector3f& point) const {
    // Apply rotation
    float rotatedX = point.x * cos(angleY_) - point.z * sin(angleY_);
    float rotatedZ = point.x * sin(angleY_) + point.z * cos(angleY_);
    float rotatedY = point.y * cos(angleX_) - rotatedZ * sin(angleX_);
    rotatedZ = point.y * sin(angleX_) + rotatedZ * cos(angleX_);

    // Apply perspective
    float perspective = CAMERA_DISTANCE * 10 / (CAMERA_DISTANCE + rotatedZ);
    float screenX = rotatedX * perspective + window_.getSize().x / 2;
    float screenY = -rotatedY * perspective + window_.getSize().y / 2;
    
    return sf::Vector2f(screenX, screenY);
}

void Visualization::handleEvents(Body& body) {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();
    }

    // Rotation control
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) angleY_ -= 0.01f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angleY_ += 0.01f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) angleX_ -= 0.01f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) angleX_ += 0.01f;

    // Movement control
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) body.move(0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) body.move(1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) body.move(2);
}

void Visualization::render(const std::vector<Point>& points, const Body& body) {
    window_.clear(sf::Color::Black);

    // Draw reference points
    for (const auto& point : points) {
        const auto& coords = point.getCoordinates();
        if (coords.size() != 3) continue;
        
        sf::Vector3f pos(coords[0], coords[1], coords[2]);
        sf::Vector2f screenPos = projectPoint(pos);
        
        sf::CircleShape circle(5.0f);
        circle.setFillColor(sf::Color::Green);
        circle.setPosition(screenPos);
        circle.setOrigin(5.0f, 5.0f);
        window_.draw(circle);
    }

    // Draw body
    const auto& bodyCoords = body.getCoordinates();
    if (bodyCoords.size() == 3) {
        sf::Vector3f bodyPos(bodyCoords[0], bodyCoords[1], bodyCoords[2]);
        sf::Vector2f bodyScreenPos = projectPoint(bodyPos);
        
        sf::CircleShape bodyCircle(8.0f);
        bodyCircle.setFillColor(sf::Color::Red);
        bodyCircle.setPosition(bodyScreenPos);
        bodyCircle.setOrigin(8.0f, 8.0f);
        window_.draw(bodyCircle);
    }

    window_.display();
}

void Visualization::run(const std::vector<Point>& points, Body& body) {
    while (window_.isOpen()) {
        handleEvents(body);
        render(points, body);
        
        // Calculate error
        double error = body.calculateError(points);
        std::cout << "Current error: " << error << std::endl;
    }
}