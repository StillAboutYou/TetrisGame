#include "tetrisgame.h"
#include <clocale>

int main() {
    setlocale(LC_ALL, "Russian");
    TetrisGame game(10, 8);
    game.startGame();
    return 0;
}