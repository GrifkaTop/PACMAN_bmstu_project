// Point.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GameConstants.hpp"

class Point {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    bool isActive;

public:
    explicit Point(const sf::Vector2f& pos);
    
    void draw(sf::RenderWindow& window) const;
    bool checkCollision(const sf::Vector2f& playerPos) const;
    void collect();
    bool isCollected() const;
};