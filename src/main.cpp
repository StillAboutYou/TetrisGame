#include "tetrisgame.h"
#include <clocale>
#include <iostream>

int main() {
    setlocale(LC_ALL, "Russian");
    TetrisGame game(20, 10);
    while (true) {
        game.startGame();
        std::cout << "Вы проирали, ваш счёт: " << game.getScore() << "\n";
        std::cout << "1. Начать заново\n";
        std::cout << "2. Главное меню\n";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            continue;
        }
        else {
            break;
        }
    }
    return 0;
}