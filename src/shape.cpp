#include "shape.h"
#include "tetrisgame.h"

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
        y += static_cast<int8_t>(dy * y_speed);
        x += dx;
    }
    // checkCollision(); закомментил пока что т.к. реализовано неверно
}

void Shape::checkCollision() {
    const auto& board = game.getBoard();
    for (const auto& [y, x] : coords) {
        if (x < 0 || x >= board[0].size() || y >= board.size()) {
            coords = old_coords;
            return;
        }
        if (y >= 0 && board[y][x] != ' ') {
            coords = old_coords;
            return;
        }
    }
}