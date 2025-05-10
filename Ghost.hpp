#pragma once
#include "Unit.hpp"
#include "GameConstants.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>

class Ghost : public Unit {
private:
    sf::CircleShape body;
    sf::Vector2f position;
    std::vector<sf::Vector2i> currentPath;
    size_t currentPathIndex;
    float pathUpdateTimer;

    struct Node {
        sf::Vector2i pos;
        float cost;
        float heuristic;
        bool operator<(const Node& other) const {
            return (cost + heuristic) > (other.cost + other.heuristic);
        }
    };

    void updatePath(const Map& map, const sf::Vector2f& targetPos);
    void reconstructPath(const std::unordered_map<int, std::unordered_map<int, sf::Vector2i>>& cameFrom, sf::Vector2i current);

public:
    explicit Ghost(const sf::Vector2f& startPosition);
    
    void update(const Map& map, const sf::Vector2f& targetPos, float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    bool checkCollision(const sf::Vector2f& pacmanPos) const;
    sf::Vector2f getPosition() const override;
};