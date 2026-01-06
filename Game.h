#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Player;
class Map;
class Raycaster;

class Game {
public:
    Game(sf::RenderWindow& window);
    ~Game();
    void run();

private:
    sf::RenderWindow& window;
    Player* player;
    Map* map;
    Raycaster* raycaster;
    int roomCount;
    bool gameWon;
    bool gameLost;
    bool isPaused;
    float playerHealth;
    float maxHealth;
    bool playerIsHurt;
    sf::Vector2i centerMouse;
    sf::Font font;

    sf::Text healthText;
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBar;
    sf::Text roomText;

    void update(float deltaTime);
    void render();
    void handleInput(float deltaTime);
    void generateNewRoom();
    void renderHUD();
    void resetGame();
};

#endif