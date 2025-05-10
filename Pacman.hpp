#pragma once
#include "Unit.hpp"
#include "GameConstants.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Pacman : public Unit {
private:
    sf::CircleShape body;
    sf::Vector2f position;
    sf::Vector2f direction;
    sf::Vector2f nextDirection;
    float animationPhase;
    bool isAtIntersection;
    sf::Vector2i currentTile;
    std::vector<sf::Vector2i> availableDirections; // Добавлено

    bool checkCollision(const Map& map, const sf::Vector2f& pos) const;
    void updateIntersectionState(const Map& map);

public:
    explicit Pacman(const sf::Vector2f& startPosition);

    void handleInput(const sf::Event& event, const Map& map); // Добавлен параметр Map
    void update(const Map& map, const sf::Vector2f& targetPos, float deltaTime) override;    
    void draw(sf::RenderWindow& window) const override;
    sf::Vector2f getPosition() const override;
};