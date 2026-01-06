#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "Player.h"

class Raycaster {
public:
    Raycaster(sf::RenderWindow& window, const Map& map);
    void render(const Player& player, const Map& map, float playerHealth, bool isHurt);

private:
    sf::RenderWindow& window;
    const Map& map;
    int screenWidth, screenHeight;
    sf::RectangleShape hurtOverlay; // Ёффект крови при получении урона
    sf::Clock hurtTimer;
    bool showHurtEffect;
};

#endif