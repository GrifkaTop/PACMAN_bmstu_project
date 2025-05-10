#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Point.hpp"

class Map {
private:
    std::vector<std::vector<int>> grid;
    int width;
    int height;
    std::vector<Point> points;

public:
    Map();
    
    bool loadFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window) const;
    int getWidth() const;
    int getHeight() const;
    int getCell(int x, int y) const;
    sf::Vector2i findObjectPosition(int objectType) const;
    
    // Добавляем недостающие методы
    sf::Vector2i getTilePosition(const sf::Vector2f& worldPos) const;
    sf::Vector2f getWorldPosition(const sf::Vector2i& tilePos) const;
    sf::Vector2f alignToGrid(const sf::Vector2f& worldPos) const;
    std::vector<sf::Vector2i> getAvailableDirections(const sf::Vector2i& tilePos) const;
    std::vector<Point>& getPoints();
    void updatePoints();
};