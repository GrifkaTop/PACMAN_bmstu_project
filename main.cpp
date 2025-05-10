#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "GameConstants.hpp"

int main() {
    // Инициализация
    sf::RenderWindow window;
    Map gameMap;
    sf::Font mainFont;
    
    // Переменные состояния
    bool gameStarted = false;
    bool gameRunning = false;
    bool gameWon = false;
    int score = 0;

    // Загрузка карты
    if (!gameMap.loadFromFile("map.txt")) {
        std::cerr << "Failed to load map file!" << std::endl;
        return 1;
    }

    // Создание окна
    const int windowWidth = gameMap.getWidth() * GameConstants::TILE_SIZE;
    const int windowHeight = gameMap.getHeight() * GameConstants::TILE_SIZE;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Pacman");

    // Загрузка шрифта
    if (!mainFont.loadFromFile("ArialBlack.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Инициализация текста
    sf::Text startText;
    startText.setFont(mainFont);
    startText.setString("Press SPACE to start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::Yellow);
    startText.setPosition(
        windowWidth/2 - startText.getLocalBounds().width/2,
        windowHeight/2 - 50
    );

    sf::Text scoreText;
    scoreText.setFont(mainFont);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 5);

    // Создание персонажей
    Pacman pacman(gameMap.getWorldPosition(gameMap.findObjectPosition(2)));
    Ghost ghost(gameMap.getWorldPosition(gameMap.findObjectPosition(3)));

    sf::Clock gameClock;
    
    // Главный цикл
    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        sf::Event event;

        // Обработка событий
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Старт/рестарт игры
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!gameStarted) {
                    gameStarted = true;
                    gameRunning = true;
                } else if (!gameRunning) {
                    // Рестарт
                    gameMap.loadFromFile("map.txt");
                    pacman = Pacman(gameMap.getWorldPosition(gameMap.findObjectPosition(2)));
                    ghost = Ghost(gameMap.getWorldPosition(gameMap.findObjectPosition(3)));
                    score = 0;
                    gameWon = false;
                    gameRunning = true;
                }
            }

            if (gameRunning) {
                pacman.handleInput(event, gameMap);
            }
        }

        if (gameRunning) {
            // Обновление
            pacman.update(gameMap, sf::Vector2f(), deltaTime);
            ghost.update(gameMap, pacman.getPosition(), deltaTime);

            // Сбор точек
            for (auto& point : gameMap.getPoints()) {
                if (point.checkCollision(pacman.getPosition())) {
                    point.collect();
                    score += GameConstants::POINT_SCORE_VALUE;
                }
            }
            gameMap.updatePoints();

            // Проверка победы
            if (gameMap.getPoints().empty()) {
                gameWon = true;
                gameRunning = false;
            }

            // Проверка поражения
            if (ghost.checkCollision(pacman.getPosition())) {
                gameRunning = false;
            }
        }

        // Отрисовка
        window.clear(sf::Color::Black);

        // Отрисовка карты и персонажей
        gameMap.draw(window);
        pacman.draw(window);
        ghost.draw(window);

        // Интерфейс
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        // Экран старта
        if (!gameStarted) {
            window.draw(startText);
        }

        // Экран завершения
        if (!gameRunning && gameStarted) {
            sf::Text endText;
            endText.setFont(mainFont);
            endText.setCharacterSize(50);
            endText.setPosition(
                windowWidth/2 - endText.getLocalBounds().width/2,
                windowHeight/2 - 50
            );

            if (gameWon) {
                endText.setString("YOU WIN! Press SPACE to restart");
                endText.setFillColor(sf::Color::Green);
            } else {
                endText.setString("GAME OVER! Press SPACE to restart");
                endText.setFillColor(sf::Color::Red);
            }
            
            window.draw(endText);
        }

        window.display();
    }

    return 0;
}