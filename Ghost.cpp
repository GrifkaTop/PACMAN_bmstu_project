#include "Ghost.hpp"
#include "Map.hpp"
#include "GameConstants.hpp"

using namespace GameConstants;

Ghost::Ghost(const sf::Vector2f& startPosition) 
    : position(startPosition), currentPathIndex(0), pathUpdateTimer(0) {
    
    body.setRadius(GHOST_RADIUS);
    body.setFillColor(sf::Color(GHOST_COLOR));
    body.setOrigin(GHOST_RADIUS, GHOST_RADIUS);
    body.setPosition(position);
}

void Ghost::update(const Map& map, const sf::Vector2f& targetPos, float deltaTime) {
    pathUpdateTimer += deltaTime;

    // Обновляем путь каждые GHOST_PATH_UPDATE_TIME секунд
    if (pathUpdateTimer >= GHOST_PATH_UPDATE_TIME) {
        updatePath(map, targetPos);
        pathUpdateTimer = 0;
    }

    // Двигаемся по пути
    if (!currentPath.empty() && currentPathIndex < currentPath.size()) {
        sf::Vector2f targetWorld = map.getWorldPosition(currentPath[currentPathIndex]);
        sf::Vector2f direction = targetWorld - position;
        float distance = std::hypot(direction.x, direction.y);

        if (distance < 5.0f) {
            currentPathIndex++;
        } else {
            direction /= distance;
            position += direction * GHOST_SPEED * deltaTime * 60.0f;
        }
    }
    body.setPosition(position);
}

void Ghost::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

bool Ghost::checkCollision(const sf::Vector2f& pacmanPos) const {
    float distance = std::hypot(
        position.x - pacmanPos.x,
        position.y - pacmanPos.y
    );
    return distance < GHOST_KILL_DISTANCE;
}

void Ghost::updatePath(const Map& map, const sf::Vector2f& targetPos) {
    sf::Vector2i start = map.getTilePosition(position);
    sf::Vector2i end = map.getTilePosition(targetPos);

    // Проверка валидности конечной точки
    if (end.x < 0 || end.x >= map.getWidth() || 
        end.y < 0 || end.y >= map.getHeight() ||
        map.getCell(end.x, end.y) == 1) {
        return;
    }

    std::priority_queue<Node> openSet;
    std::unordered_map<int, std::unordered_map<int, bool>> closedSet;
    std::unordered_map<int, std::unordered_map<int, sf::Vector2i>> cameFrom;
    std::unordered_map<int, std::unordered_map<int, float>> gScore;

    auto heuristic = [](const sf::Vector2i& a, const sf::Vector2i& b) {
        return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
    };

    openSet.push({start, 0.0f, heuristic(start, end)});
    gScore[start.x][start.y] = 0.0f;

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.pos == end) {
            reconstructPath(cameFrom, current.pos);
            return;
        }

        if (closedSet[current.pos.x][current.pos.y]) continue;
        closedSet[current.pos.x][current.pos.y] = true;

        const int dx[] = {1, -1, 0, 0};
        const int dy[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; ++i) {
            sf::Vector2i neighbor = current.pos + sf::Vector2i(dx[i], dy[i]);

            if (neighbor.x < 0 || neighbor.x >= map.getWidth() ||
                neighbor.y < 0 || neighbor.y >= map.getHeight()) continue;

            if (map.getCell(neighbor.x, neighbor.y) == 1) continue;

            float tentativeG = gScore[current.pos.x][current.pos.y] + 1.0f;

            if (!gScore[neighbor.x].count(neighbor.y) || 
                tentativeG < gScore[neighbor.x][neighbor.y]) {
                
                cameFrom[neighbor.x][neighbor.y] = current.pos;
                gScore[neighbor.x][neighbor.y] = tentativeG;
                float h = heuristic(neighbor, end);
                openSet.push({neighbor, tentativeG, h});
            }
        }
    }
}

void Ghost::reconstructPath(
    const std::unordered_map<int, std::unordered_map<int, sf::Vector2i>>& cameFrom,
    sf::Vector2i current) {
    
    currentPath.clear();
    while (cameFrom.count(current.x) && cameFrom.at(current.x).count(current.y)) {
        currentPath.push_back(current);
        current = cameFrom.at(current.x).at(current.y);
    }
    std::reverse(currentPath.begin(), currentPath.end());
    currentPathIndex = 0;
}

sf::Vector2f Ghost::getPosition() const {
    return position;
}