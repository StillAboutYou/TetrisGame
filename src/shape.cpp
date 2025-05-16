#include "shape.h"
#include "tetrisgame.h"
#include <iostream>

Shape::Shape(TetrisGame& game_ref) : game(game_ref), xpos(game_ref.getBoard()[0].size() / 2) {
    type = rand() % 7;
    int start_y = 0;

    switch (type) {
    case 0: // O
        orientations = { {{0,0}, {0,1}, {1,0}, {1,1}} };
        symbol = 'O';
        start_y = -2;
        color = 14; // Жёлтый
        break;
    case 1: // I
        orientations = {
            {{0,0}, {0,1}, {0,2}, {0,3}},
            {{0,0}, {1,0}, {2,0}, {3,0}}
        };
        symbol = 'I';
        start_y = -4;
        color = 11; // Голубой
        break;
    case 2: // S
        orientations = {
            {{0,1}, {0,2}, {1,0}, {1,1}}, // Горизонтальная S
            {{0,0}, {1,0}, {1,1}, {2,1}}  // Вертикальная S
        };
        symbol = 'S';
        start_y = -2;
        color = 12; // Красный
        break;
    case 3: // Z
        orientations = {
            {{0,0}, {0,1}, {1,1}, {1,2}}, // Горизонтальная Z
            {{0,1}, {1,0}, {1,1}, {2,0}}  // Вертикальная Z
        };
        symbol = 'Z';
        start_y = -2;
        color = 10; // Зелёный
        break;
    case 4: // L
        orientations = {
            {{0,0}, {1,0}, {2,0}, {2,1}}, // Основная
            {{0,2}, {1,0}, {1,1}, {1,2}}, // Поворот 1
            {{0,0}, {0,1}, {1,1}, {2,1}}, // Поворот 2
            {{0,0}, {0,1}, {0,2}, {1,0}}  // Поворот 3
        };
        symbol = 'L';
        start_y = -3;
        color = 13; // Пурпурный
        break;
    case 5: // J
        orientations = {
            {{0,1}, {1,1}, {2,1}, {2,0}}, // Основная
            {{0,0}, {1,0}, {1,1}, {1,2}}, // Поворот 1
            {{0,0}, {0,1}, {1,0}, {2,0}}, // Поворот 2
            {{0,0}, {0,1}, {0,2}, {1,2}}  // Поворот 3
        };
        symbol = 'J';
        start_y = -3;
        color = 9;  // Синий
        break;
    case 6: // T
        orientations = {
            {{0,0}, {0,1}, {0,2}, {1,1}},
            {{0,1}, {1,0}, {1,1}, {2,1}},
            {{1,0}, {1,1}, {1,2}, {0,1}},
            {{0,0}, {1,0}, {1,1}, {2,0}}
        };
        symbol = 'T';
        start_y = -2;
        color = 15; // Белый
        break;
    }

    // Установка начальных координат
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(start_y + rel_y, xpos + rel_x);
    }
}

bool Shape::tryWallKick(TetrisGame& game, const std::vector<std::pair<int, int>>& new_coords) {
    // Список сдвигов для проверки: (dy, dx)
    const std::vector<std::pair<int, int>> kicks = {
        {0, -1},  // влево
        {0, 1},   // вправо
        {-1, 0},  // вверх (редко, но возможно)
        {1, 0}    // вниз
    };

    for (const auto& [dy, dx] : kicks) {
        std::vector<std::pair<int, int>> kicked_coords;
        for (const auto& [y, x] : new_coords) {
            kicked_coords.emplace_back(y + dy, x + dx);
        }
        if (!checkCollisionWithCoords(kicked_coords)) {
            coords = kicked_coords;
            return true;
        }
    }
    return false;
}

void Shape::rotate() {
    uint8_t new_orientation = (orientation + 1) % orientations.size();
    std::vector<std::pair<int, int>> new_coords;
    int base_y = coords[0].first;
    int base_x = coords[0].second;
    for (const auto& [rel_y, rel_x] : orientations[new_orientation]) {
        new_coords.emplace_back(base_y + rel_y, base_x + rel_x);
    }

    if (!checkCollisionWithCoords(new_coords)) {
        coords = new_coords;
        orientation = new_orientation;
    }
    else {
        // Пробуем сдвинуть фигуру (wallkick)
        if (tryWallKick(game, new_coords)) {
            orientation = new_orientation;
        }
    }
}

void Shape::move(int dy, int dx) {
    old_coords = coords;
    for (auto& [y, x] : coords) {
        y += static_cast<int>(dy * y_speed);
        x += dx;
    }
    
    if (checkCollision()) {
        game.fixShape(*this); 
    }
}

bool Shape::checkCollision() {
    const auto& board = game.getBoard();
    /*
    std::cout << "Координаты: ";
    for (const auto& [y, x] : coords) {
        std::cout << "(" << static_cast<int>(y) << ", " << static_cast<int>(x) << ") ";
    }
    std::cout << "\n";
    */
    for (const auto& [y, x] : coords) {
        if (x < 0 || x >= static_cast<int>(board[0].size())) {
            // std::cout << "Столкновение в: (" << static_cast<int>(y) << ", " << static_cast<int>(x) << ")\n";
            coords = old_coords;
            return false;
        }
        if ((y >= static_cast<int>(board.size())) ||
           (y >= 0 && board[y][x].symbol != ' '))
        {
            // std::cout << "Столкновение в: (" << static_cast<int>(y) << ", " << static_cast<int>(x) << ")\n";
            coords = old_coords;
            return true;
        }
    }
    return false;
}

bool Shape::checkCollisionWithCoords(const std::vector<std::pair<int, int>>& test_coords) const {
    for (const auto& [y, x] : test_coords) {
        if (y >= static_cast<int>(game.getBoard().size()) || x < 0 || x >= static_cast<int>(game.getBoard()[0].size())) {
            return true;
        }
        if (y >= 0 && game.getBoard()[y][x].symbol != ' ') {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> Shape::getNormalizedCoords() const {
    int min_y = coords[0].first;
    int min_x = coords[0].second;
    for (const auto& [y, x] : coords) {
        if (y < min_y) min_y = y;
        if (x < min_x) min_x = x;
    }

    std::vector<std::pair<int, int>> normalized;
    for (const auto& [y, x] : coords) {
        normalized.emplace_back(y - min_y, x - min_x);
    }
    return normalized;
}