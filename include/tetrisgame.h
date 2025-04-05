#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include "shape.h"

class TetrisGame {
private:
    std::unique_ptr<Shape> current_shape;
    uint8_t height, width;
    std::vector<std::vector<char>> board;
    const uint8_t kFPS = 1;

    using clock = std::chrono::steady_clock;
    std::chrono::time_point<clock> last_frame_time;
    std::chrono::milliseconds frame_duration;

    void create_new_shape();
    
public:
    TetrisGame(uint8_t map_height, uint8_t map_width);
    std::vector<std::vector<char>>& getBoard() { return board; }
    void drawMap();
    void startGame();
};