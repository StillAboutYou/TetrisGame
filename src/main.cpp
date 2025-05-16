#include "tetrisgame.h"
#include <clocale>
#include <iostream>
#include <windows.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // После завершения программы в окне Output Visual Studio покажет все утечки памяти с номерами аллокаций, если они есть.
    // SetConsoleOutputCP(CP_UTF8); // Устанавливаем UTF-8
    // SetConsoleCP(CP_UTF8);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    TetrisGame game(20, 10);
    while (true) {
        std::cout << "1. Начать игру\n";
        std::cout << "2. Статистика\n";
        std::cout << "3. Выход\n";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            game.startGame();
            std::cout << "Вы проиграли, ваш счёт: " << game.getScore() << "\n";
        }
        else if (choice == 2) {
            std::cout << "Статистика:\n";
            std::cout << "Собранные линии: " << game.getTotalLinesCleared() << "\n";
            std::cout << "Упавшие фигуры: " << game.getTotalShapesDropped() << "\n";
            std::cout << "Рекорд: " << game.getHighScore() << "\n";
        }
        else {
            break;
        }
    }
    return 0;
}