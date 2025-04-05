#pragma once
#include <vector>
#include <cstdint>
#include <utility>


class TetrisGame;

class Shape {
private:
    TetrisGame& game;
    std::vector<std::pair<int, int>> old_coords;
    std::vector<std::pair<int, int>> coords;
    int xpos;
    char symbol;
    uint8_t type;
    float y_speed = 1;
    
public:
    Shape(TetrisGame& game_ref);
    const auto& get_coords() const { return coords; }
    char get_symbol() const { return symbol; }
    void rotate();
    void move(int dy = 0, int dx = 0);
    bool checkCollision();
};