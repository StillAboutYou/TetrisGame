#include "tetrisgame.h"
#include <iostream>
#include <format>
#include <conio.h>
#include <thread>
#include <windows.h>
#include <ctime>  // Добавьте этот заголовочный файл

TetrisGame::TetrisGame(uint8_t map_height, uint8_t map_width) 
    : height(map_height), width(map_width), 
      board(map_height, std::vector<Cell>(map_width, Cell{' ', 7})),
      frame_duration(1000 / kFPS) {
    last_frame_time = clock::now();    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Инициализация рандомного сида
}

void TetrisGame::create_new_shape() {
    current_shape = std::move(next_shape);
    switch (rand() % 7) {
    case 0: next_shape = std::make_unique<OShape>(*this); break;
    case 1: next_shape = std::make_unique<IShape>(*this); break;
    case 2: next_shape = std::make_unique<SShape>(*this); break;
    case 3: next_shape = std::make_unique<ZShape>(*this); break;
    case 4: next_shape = std::make_unique<LShape>(*this); break;
    case 5: next_shape = std::make_unique<JShape>(*this); break;
    case 6: next_shape = std::make_unique<TShape>(*this); break;
    }
    if (current_shape) { // Проверка на nullptr
        auto test_coords = current_shape->get_coords();
        for (auto& [y, x] : test_coords) {
            y += static_cast<int>(1 * current_shape->get_y_speed());
            x += 0;
        }
        if (current_shape->checkCollisionWithCoords(test_coords)) {
            is_game = false;
        }
    }
}

void TetrisGame::fixShape(const Shape& shape) {
    for (const auto& [y, x] : shape.get_coords()) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            board[y][x].symbol = shape.get_symbol();
            board[y][x].color = shape.get_color();
        }
    }
    totalShapesDropped++;
    checkLines();
    create_new_shape();
}

void TetrisGame::checkLines() {
    int lines_cleared = 0;
    for (int y = height - 1; y >= 0; --y) {
        bool isFull = true;
        for (int x = 0; x < width; ++x) {
            if (board[y][x].symbol == ' ') {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            board.erase(board.begin() + y);
            board.insert(board.begin(), std::vector<Cell>(width, Cell{' ', 7}));
            lines_cleared++;
            y++; // проверяем ту же строку снова
        }
    }
    if (lines_cleared > 0) {
        switch (lines_cleared) {
        case 1: score += 100; break;
        case 2: score += 300; break;
        case 3: score += 500; break;
        case 4: score += 800; break;
        }
    }

    totalLinesCleared += lines_cleared;
}

void TetrisGame::drawMap() {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    auto temp_board = board;

    for (const auto& [y, x] : current_shape->get_coords()) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            temp_board[y][x].symbol = current_shape->get_symbol();
            temp_board[y][x].color = current_shape->get_color();
        }
    }
    
    SetConsoleTextAttribute(hConsole, 7); // Белый цвет для рамки
    std::cout << "  ";
    for (int x = 0; x < width + 2; x++) std::cout << "-";
    std::cout << "   Следующая фигура:\n";

    auto normalized_coords = next_shape->getNormalizedCoords();

    for (int y = 0; y < height; y++) {
        SetConsoleTextAttribute(hConsole, 7); // Белый цвет для рамки
        std::cout << "  |";
        for (int x = 0; x < width; x++) {
            if (temp_board[y][x].symbol != ' ') {
                SetConsoleTextAttribute(hConsole, temp_board[y][x].color);
                std::cout << temp_board[y][x].symbol;
            }
            else {
                SetConsoleTextAttribute(hConsole, 8); // Серый для точек
                std::cout << '.';
            }
        }

        SetConsoleTextAttribute(hConsole, 7);
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
                if (isNextShape) {
                    SetConsoleTextAttribute(hConsole, temp_board[y][x].color);
                    std::cout << next_shape->get_symbol();
                }
                else {
                    SetConsoleTextAttribute(hConsole, 8); // Серый для точек
                    std::cout << '.';
                }
            }
        }
        std::cout << "\n";
    }

    std::cout << "  ";
    for (int x = 0; x < width + 2; ++x) std::cout << "-";
    std::cout << "\n";
    std::cout << "  Счёт: " << score << "\n";

    // Написание координат текущей фигуры (отладка)
    // может пригодиться для написание rotate()

    std::cout << "Current shape coordinates:\n";
    for (const auto& [y, x] : current_shape->get_coords()) {
        std::cout << "(" << static_cast<int>(y) << ", " << static_cast<int>(x) << ") ";
    }

    //

}

void TetrisGame::resetGame() {
    board = std::vector<std::vector<Cell>>(height, std::vector<Cell>(width, Cell{ ' ', 7 }));
    score = 0;
    totalLinesCleared = 0;
    totalShapesDropped = 0;
    current_shape.reset(); // Сбрасываем текущую фигуру
    next_shape.reset();    // Сбрасываем следующую фигуру

    // Инициализируем next_shape случайной фигурой
    switch (rand() % 7) {
    case 0: next_shape = std::make_unique<OShape>(*this); break;
    case 1: next_shape = std::make_unique<IShape>(*this); break;
    case 2: next_shape = std::make_unique<SShape>(*this); break;
    case 3: next_shape = std::make_unique<ZShape>(*this); break;
    case 4: next_shape = std::make_unique<LShape>(*this); break;
    case 5: next_shape = std::make_unique<JShape>(*this); break;
    case 6: next_shape = std::make_unique<TShape>(*this); break;
    }

    create_new_shape(); // Теперь current_shape будет инициализирована
    is_game = true;
}

void TetrisGame::startGame() {
    resetGame();
    while (is_game) {
        auto frame_start = clock::now();

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
    if (score > highScore) highScore = score;
}