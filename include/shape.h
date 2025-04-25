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
    float y_speed = 1.0f;
    
public:
    uint8_t type;
    Shape(TetrisGame& game_ref);
    const auto& get_coords() const { return coords; }
    float get_y_speed() const { return y_speed; }
    std::vector<std::pair<int, int>> getNormalizedCoords() const;
    char get_symbol() const { return symbol; }
    void rotate();
    void move(int dy = 0, int dx = 0);
    bool checkCollisionWithCoords(const std::vector<std::pair<int, int>>& test_coords) const;
    bool checkCollision();
};