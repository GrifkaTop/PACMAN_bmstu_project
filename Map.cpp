#include "Map.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>


using namespace GameConstants;

Map::Map() : width(0), height(0) {}

bool Map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Читаем размеры карты
    std::string line;
    if (!std::getline(file, line)) {
        return false;
    }
    
    std::istringstream sizeStream(line);
    if (!(sizeStream >> height >> width)) {
        return false;
    }

    // Читаем саму карту
    grid.resize(height, std::vector<int>(width, 0));
    
    for (int y = 0; y < height; ++y) {
        if (!std::getline(file, line) || line.size() != static_cast<size_t>(width)) {
            return false;
        }

        for (int x = 0; x < width; ++x) {
            switch (line[x]) {
                case '#': grid[y][x] = 1; break;
                case 'P': grid[y][x] = 2; break;
                case 'V': grid[y][x] = 3; break;
                case '_': grid[y][x] = 0; break;
                default: return false; // Недопустимый символ
            }
        }
    }
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == 0) { // Только для пустых клеток
                points.emplace_back(getWorldPosition({x, y}));
            }
        }
    }

    return true;
}


void Map::updatePoints() {
    // Удаляем собранные точки
    points.erase(
        std::remove_if(points.begin(), points.end(),
            [](const Point& p) { return p.isCollected(); }),
        points.end()
    );
}



void Map::draw(sf::RenderWindow& window) const {
    sf::RectangleShape wall(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wall.setFillColor(sf::Color::Blue);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == 1) {
                wall.setPosition(
                    x * TILE_SIZE,
                    y * TILE_SIZE
                );
                window.draw(wall);
            }
        }
    }

    // Отрисовываем точки
    for (const auto& point : points) {
        point.draw(window);
    }
}

sf::Vector2f Map::alignToGrid(const sf::Vector2f& worldPos) const {
    sf::Vector2i tile = getTilePosition(worldPos);
    return {
        (tile.x * TILE_SIZE) + (TILE_SIZE / 2.0f),
        (tile.y * TILE_SIZE) + (TILE_SIZE / 2.0f)
    };
}
int Map::getCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) 
        return 1; // Считаем всё за пределами карты стеной
    return grid[y][x];
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }

sf::Vector2i Map::findObjectPosition(int objectType) const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == objectType) {
                return {x, y};
            }
        }
    }
    return {-1, -1}; // Не найдено
}

std::vector<Point>& Map::getPoints() {
    return points;
}

sf::Vector2i Map::getTilePosition(const sf::Vector2f& worldPos) const {
    return {
        static_cast<int>(worldPos.x / GameConstants::TILE_SIZE),
        static_cast<int>(worldPos.y / GameConstants::TILE_SIZE)
    };
}

sf::Vector2f Map::getWorldPosition(const sf::Vector2i& tilePos) const {
    return {
        (tilePos.x + 0.5f) * GameConstants::TILE_SIZE,
        (tilePos.y + 0.5f) * GameConstants::TILE_SIZE
    };
}



std::vector<sf::Vector2i> Map::getAvailableDirections(const sf::Vector2i& tilePos) const {
    std::vector<sf::Vector2i> directions;
    const int x = tilePos.x;
    const int y = tilePos.y;
    
    if (y > 0 && grid[y-1][x] != 1) directions.emplace_back(0, -1);  // Up
    if (y < height-1 && grid[y+1][x] != 1) directions.emplace_back(0, 1);   // Down
    if (x > 0 && grid[y][x-1] != 1) directions.emplace_back(-1, 0);  // Left
    if (x < width-1 && grid[y][x+1] != 1) directions.emplace_back(1, 0);    // Right
    
    return directions;
}