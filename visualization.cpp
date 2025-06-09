#include "visualization.h"
#include <iostream>
#include <cmath>

Visualization::Visualization() : window_(sf::VideoMode(800, 600), "3D Visualization") {
    window_.setFramerateLimit(60);
    
    // Initialize grid lines
    initializeGrid();
}

void Visualization::initializeGrid() {
    // Create X-axis grid lines
    for (int i = -50; i <= 50; i += 20) {
        for (int j = -50; j <= 50; j += 20) {
            // X-axis lines (YZ plane)
            gridLines_.emplace_back(sf::Vector3f(-50, i, j), sf::Vector3f(50, i, j));
            // Y-axis lines (XZ plane)
            gridLines_.emplace_back(sf::Vector3f(i, -50, j), sf::Vector3f(i, 50, j));
            // Z-axis lines (XY plane)
            gridLines_.emplace_back(sf::Vector3f(i, j, -50), sf::Vector3f(i, j, 50));
        }
    }
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) body.move(1, 1.0f);   // Вверх
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) body.move(1, -1.0f);  // Вниз
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) body.move(0, -1.0f);  // Влево
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) body.move(0, 1.0f);   // Вправо
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) body.move(2, -1.0f);  // Назад
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) body.move(2, 1.0f);   // Вперед
}

void Visualization::drawLine(const sf::Vector3f& start, const sf::Vector3f& end, sf::Color color) {
    sf::Vector2f screenStart = projectPoint(start);
    sf::Vector2f screenEnd = projectPoint(end);
    
    sf::Vertex line[] = {
        sf::Vertex(screenStart, color),
        sf::Vertex(screenEnd, color)
    };
    
    window_.draw(line, 2, sf::Lines);
}

void Visualization::render(const std::vector<Point>& points, const Body& body) {
    window_.clear(sf::Color::Black);

    // Draw grid lines
    for (const auto& line : gridLines_) {
        drawLine(line.first, line.second, sf::Color(50, 50, 50)); // Dark gray grid
    }

    // Draw coordinate axes
    drawLine(sf::Vector3f(0, 0, 0), sf::Vector3f(50, 0, 0), sf::Color::Red);   // X-axis
    drawLine(sf::Vector3f(0, 0, 0), sf::Vector3f(0, 50, 0), sf::Color::Green); // Y-axis
    drawLine(sf::Vector3f(0, 0, 0), sf::Vector3f(0, 0, 50), sf::Color::Blue);  // Z-axis

    // Draw reference points and lines to body
    const auto& bodyCoords = body.getCoordinates();
    if (bodyCoords.size() == 3) {
        sf::Vector3f bodyPos(bodyCoords[0], bodyCoords[1], bodyCoords[2]);
        
        for (const auto& point : points) {
            const auto& coords = point.getCoordinates();
            if (coords.size() != 3) continue;
            
            sf::Vector3f pos(coords[0], coords[1], coords[2]);
            sf::Vector2f screenPos = projectPoint(pos);
            
            // Draw line from point to body
            drawLine(pos, bodyPos, sf::Color(100, 100, 255, 150)); // Semi-transparent blue
            
            // Draw the point
            sf::CircleShape circle(5.0f);
            circle.setFillColor(sf::Color::Green);
            circle.setPosition(screenPos);
            circle.setOrigin(5.0f, 5.0f);
            window_.draw(circle);
        }

        // Draw body
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
	std::cout << "Initial points coordinates:"<<std::endl;
            for (const auto& point : points) {
                const auto& coords = point.getCoordinates();
                std::cout << "(";
                for (size_t i = 0; i < coords.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << coords[i];
                }
                std::cout << ")" <<std::endl;;
            }

    }
}