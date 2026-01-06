#include <SFML/Graphics.hpp>
#include <locale>
#include <iostream>
#include <string>

#include "Game.h"

void showMenu() {
    int choice = 0;

    while (true) {
        std::cout << "\n==============================\n";
        std::cout << "    ESCAPE FROM DURKA\n";
        std::cout << "==============================\n";
        std::cout << "1. Start Game\n";
        std::cout << "2. View Controls\n";
        std::cout << "3. Exit\n";
        std::cout << "==============================\n";
        std::cout << "Select option (1-3): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            std::cout << "\nLaunching game...\n";
            return;
        case 2:
            std::cout << "\n=== CONTROLS ===\n";
            std::cout << "Move: W A S D\n";
            std::cout << "Look: Mouse\n";
            std::cout << "Run: Shift\n";
            std::cout << "Pause: ESC\n";
            std::cout << "Restart: R (when dead)\n";
            std::cout << "Click: Select in menus\n";
            std::cout << "================\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        case 3:
            exit(0);
        default:
            std::cout << "Please enter 1, 2, or 3.\n";
        }
    }
}

int main() {

    showMenu();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Escape from Durka");

    // Проверка успешности создания окна
    if (!window.isOpen()) {
        std::cout << "ERROR: Failed to create game window!\n";
        std::cout << "Make sure you have graphics drivers installed.\n";
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    Game game(window);
    game.run();

    return 0;
}