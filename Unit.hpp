// Unit.h
#pragma once
#include "Map.hpp"
#include "GameConstants.hpp"
#include <SFML/Graphics.hpp>

class Unit {
public:
    virtual void update(const Map& map, const sf::Vector2f& targetPos, float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual ~Unit() = default;
};