#pragma once

namespace GameConstants {
    // Размеры и параметры объектов
    constexpr int TILE_SIZE = 32;          // Размер одной клетки карты в пикселях
    constexpr float PACMAN_RADIUS = TILE_SIZE/2  - 5.0f;
    constexpr float GHOST_RADIUS = TILE_SIZE/2  - 5.0f;
    // Скорости движения
    constexpr float PACMAN_SPEED = 100.0f;   // Скорость пакмана (пикселей/кадр)
    constexpr float GHOST_SPEED = 1.0f;    // Скорость призрака
    
    // Параметры анимации
    constexpr float PACMAN_ANIMATION_SPEED = 10.0f; // Скорость анимации рта
    
    // Логика призраков
    constexpr float GHOST_PATH_UPDATE_TIME = 0.1f;  // Интервал обновления пути (в секундах)
    constexpr float GHOST_KILL_DISTANCE =           // Дистанция для столкновения
        PACMAN_RADIUS + GHOST_RADIUS;
        
    // Цвета объектов
    constexpr unsigned WALL_COLOR = 0x0000FFFF;     // Синий (ARGB)
    constexpr unsigned PACMAN_COLOR = 0xFFFF00FF;   // Желтый
    constexpr unsigned GHOST_COLOR = 0xFF0000FF;    // Красный

    constexpr int POINT_SCORE_VALUE = 10;

    constexpr float PACMAN_GRID_SNAP_THRESHOLD = 0.1f; 

}