#include "tetrisgame.h"
#include <iostream>
#include <format>
#include <conio.h>
#include <thread>

TetrisGame::TetrisGame(uint8_t map_height, uint8_t map_width) 
    : height(map_height), width(map_width), 
      board(map_height, std::vector<char>(map_width, ' ')),
      frame_duration(1000 / kFPS) {
    create_new_shape();
    last_frame_time = clock::now();
}

void TetrisGame::create_new_shape() {
    current_shape = std::make_unique<Shape>(*this);
}

void TetrisGame::drawMap() {
    system("cls");
    auto temp_board = board;

    for (const auto& [y, x] : current_shape->get_coords()) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            temp_board[y][x] = current_shape->get_symbol();
        }
    }

    for (const auto& row : temp_board) {
        for (char cell : row) {
            std::cout << (cell == ' ' ? '.' : cell);
        }
        std::cout << "\n";
    }
}

void TetrisGame::startGame() {
    while (true) {
        auto frame_start = clock::now();
        
        drawMap();

        // Написание координат текущей фигуры (отладка)
        // может пригодиться для написание rotate()
        std::cout << "Current shape coordinates:\n";
        for (const auto& [y, x] : current_shape->get_coords()) {
            std::cout << "(" << static_cast<int>(y) << ", " << static_cast<int>(x) << ") ";
        }
        //

        current_shape->move(1, 0);

        if (_kbhit()) {
            char key = _getch();
            if (key == 'q') break;
            if (key == 'a') current_shape->move(0, -1);
            if (key == 's') current_shape->move(1, 0);
            if (key == 'd') current_shape->move(0, 1);
            if (key == 'w') current_shape->rotate();
        }

        auto frame_end = clock::now();
        auto sleep_time = frame_duration - (frame_end - frame_start);
        if (sleep_time > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}