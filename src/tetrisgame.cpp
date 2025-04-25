#include "tetrisgame.h"
#include <iostream>
#include <format>
#include <conio.h>
#include <thread>

TetrisGame::TetrisGame(uint8_t map_height, uint8_t map_width) 
    : height(map_height), width(map_width), 
      board(map_height, std::vector<char>(map_width, ' ')),
      frame_duration(1000 / kFPS) {
    last_frame_time = clock::now();
}

void TetrisGame::create_new_shape() {
    current_shape = std::move(next_shape);
    next_shape = std::make_unique<Shape>(*this);
    
    auto test_coords = current_shape->get_coords();
    for (auto& [y, x] : test_coords) {
        y += static_cast<int>(1 * current_shape->get_y_speed());
        x += 0;
    }

    if (current_shape->checkCollisionWithCoords(test_coords)) {
        is_game = false;
    }
}

void TetrisGame::fixShape(const Shape& shape) {
    for (const auto& [y, x] : shape.get_coords()) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            board[y][x] = shape.get_symbol();
        }
    }
    checkLines();
    create_new_shape();
}

void TetrisGame::checkLines() {
    for (int y = height - 1; y >= 0; --y) {
        bool isFull = true;
        for (int x = 0; x < width; ++x) {
            if (board[y][x] == ' ') {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            board.erase(board.begin() + y);
            board.insert(board.begin(), std::vector<char>(width, ' '));
            score += 100; 
            y++; 
        }
    }
}

void TetrisGame::drawMap() {
    system("cls");
    auto temp_board = board;

    for (const auto& [y, x] : current_shape->get_coords()) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            temp_board[y][x] = current_shape->get_symbol();
        }
    }

    std::cout << "  ";
    for (int x = 0; x < width + 2; x++) std::cout << "-";
    std::cout << "   Следующая фигура:\n";

    auto normalized_coords = next_shape->getNormalizedCoords();

    for (int y = 0; y < height; y++) {
        std::cout << "  |";
        for (int x = 0; x < width; x++) {
            std::cout << (temp_board[y][x] == ' ' ? '.' : temp_board[y][x]);
        }
        std::cout << "|";

        if (y < 4 && next_shape) {
            std::cout << "  ";
            for (int x = 0; x < 4; x++) {
                bool isNextShape = false;
                for (const auto& [ny, nx] : normalized_coords) {
                    if (ny == y && nx == x) {
                        isNextShape = true;
                        break;
                    }
                }
                std::cout << (isNextShape ? next_shape->get_symbol() : '.');
            }
        }
        std::cout << "\n";
    }

    std::cout << "  ";
    for (int x = 0; x < width + 2; ++x) std::cout << "-";
    std::cout << "\n";
    std::cout << "  Счёт: " << score << "\n";
}

void TetrisGame::resetGame() {
    board = std::vector<std::vector<char>>(height, std::vector<char>(width, ' '));
    score = 0;
    next_shape = std::make_unique<Shape>(*this);
    create_new_shape();
    is_game = true;
}

void TetrisGame::startGame() {
    resetGame();
    while (is_game) {
        auto frame_start = clock::now();


        // Написание координат текущей фигуры (отладка)
        // может пригодиться для написание rotate()
        /*
        std::cout << "Current shape coordinates:\n";
        for (const auto& [y, x] : current_shape->get_coords()) {
            std::cout << "(" << static_cast<int>(y) << ", " << static_cast<int>(x) << ") ";
        }
        */
        //

        current_shape->move(1, 0);
        drawMap();

        if (_kbhit()) {
            char key = _getch();
            if (key == 'q') break;
            if (key == 'a') {
                current_shape->move(0, -1);
                drawMap();
            }
            if (key == 's') {
                current_shape->move(1, 0);
                drawMap();
            }
            if (key == 'd') {
                current_shape->move(0, 1);
                drawMap();
            }
            if (key == 'w') {
                current_shape->rotate();
                drawMap();
            }
        }

        auto frame_end = clock::now();
        auto sleep_time = frame_duration - (frame_end - frame_start);
        if (sleep_time > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}