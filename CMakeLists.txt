cmake_minimum_required(VERSION 3.10)
project(PacmanProject)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Исходные файлы
set(SOURCES
    main.cpp
    Point.cpp
    Map.cpp
    Pacman.cpp
    Ghost.cpp
    #Unit.cpp
)

# Заголовочные файлы
set(HEADERS
    Unit.hpp
    Map.hpp
    Pacman.hpp
    Ghost.hpp
    Point.hpp
    GameConstants.hpp
)

# Создание исполняемого файла
add_executable(Pacman ${SOURCES} ${HEADERS})

# Линковка с SFML
target_link_libraries(Pacman
    PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Оптимизация для отладки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -Wall -Wextra)
endif()