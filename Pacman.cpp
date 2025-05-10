#include "Pacman.hpp"
#include <cmath>
#include <iostream>

using namespace GameConstants;

Pacman::Pacman(const sf::Vector2f& startPosition) 
    : direction(1, 0), nextDirection(0, 0), animationPhase(0.0f),
      isAtIntersection(false), currentTile(0, 0) {
    
    body.setRadius(PACMAN_RADIUS);
    body.setFillColor(sf::Color(PACMAN_COLOR));
    body.setOrigin(PACMAN_RADIUS, PACMAN_RADIUS);
    position = startPosition;
    body.setPosition(position);
}

void Pacman::handleInput(const sf::Event& event, const Map& map) {
    if (event.type == sf::Event::KeyPressed) {
        sf::Vector2f newDir(0, 0);
        switch (event.key.code) {
            case sf::Keyboard::Up:    newDir = {0, -1}; break;
            case sf::Keyboard::Down:  newDir = {0, 1};  break;
            case sf::Keyboard::Left:  newDir = {-1, 0}; break;
            case sf::Keyboard::Right: newDir = {1, 0};  break;
            default: break;
        }

        if (newDir != sf::Vector2f(0, 0)) {
            sf::Vector2i nextTile = map.getTilePosition(position) + sf::Vector2i(newDir.x, newDir.y);
            if (map.getCell(nextTile.x, nextTile.y) != 1) {
                direction = newDir;
                std::cout << "Direction set to: (" << direction.x << ", " << direction.y << ")\n"; // Отладка
            }
        }
    }
}

void Pacman::update(const Map& map, [[maybe_unused]] const sf::Vector2f& targetPos, float deltaTime) {
    // Если direction не задан - не двигаемся
    if (direction == sf::Vector2f(0, 0)) return;

    // Рассчитываем новую позицию с учетом времени кадра
    sf::Vector2f newPosition = position + direction * PACMAN_SPEED * deltaTime;

    // Проверяем коллизии
    if (!checkCollision(map, newPosition)) {
        position = newPosition;
    } else {
        // Выравнивание при столкновении
        position = map.alignToGrid(position);
    }

    // Обновляем позицию спрайта
    body.setPosition(position);
}
void Pacman::draw(sf::RenderWindow& window) const {
    // Отрисовка основного тела
    window.draw(body);

    // Отрисовка анимации рта
    if (direction != sf::Vector2f(0, 0)) {
        sf::ConvexShape mouth(3);
        mouth.setFillColor(sf::Color::Black);
        
        // Расчет угла поворота
        float baseAngle = 0.0f;
        if (direction.x == 1) baseAngle = 0.0f;
        else if (direction.x == -1) baseAngle = 180.0f;
        else if (direction.y == 1) baseAngle = 90.0f;
        else if (direction.y == -1) baseAngle = 270.0f;

        // Анимированный угол раскрытия рта (20°-50°)
        float mouthAngle = 35.0f + 15.0f * sin(animationPhase * 3.14159f * 2.0f);
        
        // Расчет точек треугольника
        const float rad = baseAngle * 3.14159f / 180.0f;
        mouth.setPoint(0, position);
        mouth.setPoint(1, position + sf::Vector2f(
            PACMAN_RADIUS * 1.5f * cos(rad - mouthAngle/2 * 3.14159f/180.0f),
            PACMAN_RADIUS * 1.5f * sin(rad - mouthAngle/2 * 3.14159f/180.0f)
        ));
        mouth.setPoint(2, position + sf::Vector2f(
            PACMAN_RADIUS * 1.5f * cos(rad + mouthAngle/2 * 3.14159f/180.0f),
            PACMAN_RADIUS * 1.5f * sin(rad + mouthAngle/2 * 3.14159f/180.0f)
        ));

        window.draw(mouth);
    }
}

bool Pacman::checkCollision(const Map& map, const sf::Vector2f& pos) const {
    sf::Vector2i tile = map.getTilePosition(pos);
    std::cout << "Checking tile: (" << tile.x << ", " << tile.y << ")\n"; // Отладка
    return (map.getCell(tile.x, tile.y) == 1);
}

sf::Vector2f Pacman::getPosition() const {
    return position;
}

void Pacman::updateIntersectionState(const Map& map) {
    sf::Vector2i newTile = map.getTilePosition(position);
    
    if (newTile != currentTile) {
        currentTile = newTile;
        auto rawDirections = map.getAvailableDirections(currentTile);
        
        sf::Vector2f oppositeDir = direction * -1.0f;
        sf::Vector2i opposite(
            static_cast<int>(oppositeDir.x), 
            static_cast<int>(oppositeDir.y)
        );
        
        availableDirections.clear();
        for (const auto& dir : rawDirections) {
            if (dir != opposite) {
                availableDirections.push_back(dir);
            }
        }
        
        isAtIntersection = availableDirections.size() > 1;
    }
}