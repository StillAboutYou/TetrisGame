#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include "shape.h"

class TetrisGame {
private:
    std::unique_ptr<Shape> current_shape;
    std::unique_ptr<Shape> next_shape;
    uint8_t height, width;
    std::vector<std::vector<char>> board;
    const uint8_t kFPS = 10;
    int score = 0;
    bool is_game = true;

    using clock = std::chrono::steady_clock;
    std::chrono::time_point<clock> last_frame_time;
    std::chrono::milliseconds frame_duration;

    void create_new_shape();
    void checkLines();

public:
    TetrisGame(uint8_t map_height, uint8_t map_width);
    std::vector<std::vector<char>>& getBoard() { return board; }
    void drawMap();
    void startGame();
    void resetGame();
    void fixShape(const Shape& shape);
    int getScore() const { return score; }
    bool isGameOver() const { return !is_game; }
    const Shape* getNextShape() const { return next_shape.get(); } 
};