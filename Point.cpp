// Point.cpp
#include "Point.hpp"

using namespace GameConstants;

Point::Point(const sf::Vector2f& pos) 
    : position(pos), isActive(true) {
    shape.setRadius(3.0f);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(3.0f, 3.0f);
    shape.setPosition(position);
}

void Point::draw(sf::RenderWindow& window) const {
    if (isActive) window.draw(shape);
}

bool Point::checkCollision(const sf::Vector2f& playerPos) const {
    if (!isActive) return false;
    return shape.getGlobalBounds().contains(playerPos);
}

void Point::collect() {
    isActive = false;
}

bool Point::isCollected() const {
    return !isActive;
}