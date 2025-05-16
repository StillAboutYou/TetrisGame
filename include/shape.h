#pragma once
#include <vector>
#include <cstdint>
#include <utility>

class TetrisGame;

class Shape {
protected: // Доступно для производных классов
    TetrisGame& game;
    std::vector<std::pair<int, int>> coords;
    std::vector<std::pair<int, int>> old_coords;
    std::vector<std::vector<std::pair<int, int>>> orientations;
    int xpos;
    int color;
    char symbol;
    float y_speed = 1.0f;
    uint8_t orientation = 0;

public:
    Shape(TetrisGame& game_ref, char symbol, int color, int xpos);
    virtual ~Shape() = default; // Виртуальный деструктор для полиморфизма
    virtual void initializeOrientations() = 0; // Чисто виртуальный метод
    const auto& get_coords() const { return coords; }
    float get_y_speed() const { return y_speed; }
    int get_color() const { return color; }
    char get_symbol() const { return symbol; }
    std::vector<std::pair<int, int>> getNormalizedCoords() const;
    virtual void rotate(); // Виртуальный метод для поворота
    void move(int dy = 0, int dx = 0);
    bool tryWallKick(const std::vector<std::pair<int, int>>& new_coords);
    bool checkCollisionWithCoords(const std::vector<std::pair<int, int>>& test_coords) const;
    bool checkCollision();
};

class SShape : public Shape {
public:
    SShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};

class ZShape : public Shape {
public:
    ZShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};

class LShape : public Shape {
public:
    LShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};

class JShape : public Shape {
public:
    JShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};

class TShape : public Shape {
public:
    TShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};

class OShape : public Shape {
public:
    OShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};

class IShape : public Shape {
public:
    IShape(TetrisGame& game_ref);
    void initializeOrientations() override;
};