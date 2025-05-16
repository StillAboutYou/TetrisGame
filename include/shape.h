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
    std::vector<std::vector<std::pair<int, int>>> orientations;
    int xpos;   
    int color;
    char symbol;
    float y_speed = 1.0f;
    uint8_t orientation = 0; // текущая ориентация (0-3)
    
public:
    uint8_t type;
    Shape(TetrisGame& game_ref);
    const auto& get_coords() const { return coords; }
    float get_y_speed() const { return y_speed; }
    std::vector<std::pair<int, int>> getNormalizedCoords() const;
    int get_color() const { return color; }
    char get_symbol() const { return symbol; }
    void rotate();
    void move(int dy = 0, int dx = 0);
    bool tryWallKick(TetrisGame& game, const std::vector<std::pair<int, int>>& new_coords);
    bool checkCollisionWithCoords(const std::vector<std::pair<int, int>>& test_coords) const;
    bool checkCollision();
};