#include "shape.h"
#include "tetrisgame.h"
#include <iostream>

Shape::Shape(TetrisGame& game_ref, char symbol, int color, int xpos)
    : game(game_ref), symbol(symbol), color(color), xpos(xpos) {
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
    else if (tryWallKick(new_coords)) {
        orientation = new_orientation;
    }
}

bool Shape::tryWallKick(const std::vector<std::pair<int, int>>& new_coords) {
    const std::vector<std::pair<int, int>> kicks = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
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
// SShape (S-образная фигура)
SShape::SShape(TetrisGame& game_ref) : Shape(game_ref, 'S', 12, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-2 + rel_y, xpos + rel_x);
    }
}

void SShape::initializeOrientations() {
    orientations = {
        {{{0,1}, {0,2}, {1,0}, {1,1}}}, // Горизонтальная S
        {{{0,0}, {1,0}, {1,1}, {2,1}}}  // Вертикальная S
    };
}

// ZShape (Z-образная фигура)
ZShape::ZShape(TetrisGame& game_ref) : Shape(game_ref, 'Z', 10, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-2 + rel_y, xpos + rel_x);
    }
}

void ZShape::initializeOrientations() {
    orientations = {
        {{{0,0}, {0,1}, {1,1}, {1,2}}}, // Горизонтальная Z
        {{{0,1}, {1,0}, {1,1}, {2,0}}}  // Вертикальная Z
    };
}

// LShape (L-образная фигура)
LShape::LShape(TetrisGame& game_ref) : Shape(game_ref, 'L', 13, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-3 + rel_y, xpos + rel_x);
    }
}

void LShape::initializeOrientations() {
    orientations = {
        {{{0,0}, {1,0}, {2,0}, {2,1}}}, // Основная
        {{{0,2}, {1,0}, {1,1}, {1,2}}}, // Поворот 1
        {{{0,0}, {0,1}, {1,1}, {2,1}}}, // Поворот 2
        {{{0,0}, {0,1}, {0,2}, {1,0}}}  // Поворот 3
    };
}

// JShape (J-образная фигура)
JShape::JShape(TetrisGame& game_ref) : Shape(game_ref, 'J', 9, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-3 + rel_y, xpos + rel_x);
    }
}

void JShape::initializeOrientations() {
    orientations = {
        {{{0,1}, {1,1}, {2,1}, {2,0}}}, // Основная
        {{{0,0}, {1,0}, {1,1}, {1,2}}}, // Поворот 1
        {{{0,0}, {0,1}, {1,0}, {2,0}}}, // Поворот 2
        {{{0,0}, {0,1}, {0,2}, {1,2}}}  // Поворот 3
    };
}

// TShape (T-образная фигура)
TShape::TShape(TetrisGame& game_ref) : Shape(game_ref, 'T', 15, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-2 + rel_y, xpos + rel_x);
    }
}

void TShape::initializeOrientations() {
    orientations = {
        {{{0,0}, {0,1}, {0,2}, {1,1}}}, // Основная
        {{{0,1}, {1,0}, {1,1}, {2,1}}}, // Поворот 1
        {{{1,0}, {1,1}, {1,2}, {0,1}}}, // Поворот 2
        {{{0,0}, {1,0}, {1,1}, {2,0}}}  // Поворот 3
    };
}

OShape::OShape(TetrisGame& game_ref) : Shape(game_ref, 'O', 14, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-2 + rel_y, xpos + rel_x);
    }
}

void OShape::initializeOrientations() {
    orientations = { {{0,0}, {0,1}, {1,0}, {1,1}} };
}

IShape::IShape(TetrisGame& game_ref) : Shape(game_ref, 'I', 11, game_ref.getBoard()[0].size() / 2) {
    initializeOrientations();
    for (const auto& [rel_y, rel_x] : orientations[0]) {
        coords.emplace_back(-4 + rel_y, xpos + rel_x);
    }
}

void IShape::initializeOrientations() {
    orientations = { {{0,0}, {0,1}, {0,2}, {0,3}}, {{0,0}, {1,0}, {2,0}, {3,0}} };
}