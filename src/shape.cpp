#include "shape.h"
#include "tetrisgame.h"
#include <iostream>

Shape::Shape(TetrisGame& game_ref) : game(game_ref), xpos(game_ref.getBoard()[0].size() / 2) {
    // при инициализации объекта класса случайным образом выбирается
    // форма фигуры и задаются ее координаты
    type = rand() % 7;

    switch (type) {
    case 0:
        coords = { {-1, xpos - 1}, {-2, xpos - 1}, {-1, xpos}, {-2, xpos} };
        symbol = 'O';
        break;
    case 1:
        coords = { {-4, xpos}, {-3, xpos}, {-2, xpos}, {-1, xpos} };
        symbol = 'I';
        break;
    case 2:
        coords = { {-1, xpos - 1}, {-1, xpos}, {-2, xpos}, {-2, xpos + 1} };
        symbol = 'S';
        break;
    case 3:
        coords = { {-2, xpos - 1}, {-1, xpos}, {-2, xpos}, {-1, xpos + 1} };
        symbol = 'Z';
        break;
    case 4:
        coords = { {-1, xpos + 1}, {-3, xpos}, {-2, xpos}, {-1, xpos} };
        symbol = 'L';
        break;
    case 5:
        coords = { {-1, xpos - 1}, {-3, xpos}, {-2, xpos}, {-1, xpos} };
        symbol = 'J';
        break;
    case 6:
        coords = { {-2, xpos - 1}, {-2, xpos}, {-1, xpos}, {-2, xpos + 1} };
        symbol = 'T';
        break;
    }
}

void Shape::rotate() {
    // реализация поворота
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
           (y >= 0 && board[y][x] != ' '))
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
        if (y >= 0 && game.getBoard()[y][x] != ' ') {
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