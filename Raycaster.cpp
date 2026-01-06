#define _USE_MATH_DEFINES
#include "Raycaster.h"

#include <cmath>

Raycaster::Raycaster(sf::RenderWindow& window, const Map& map)
    : window(window), map(map), showHurtEffect(false) {
    screenWidth = window.getSize().x;
    screenHeight = window.getSize().y;

    // Инициализация эффекта крови (красный полупрозрачный прямоугольник)
    hurtOverlay.setSize(sf::Vector2f(static_cast<float>(screenWidth),
        static_cast<float>(screenHeight)));
    hurtOverlay.setFillColor(sf::Color(255, 0, 0, 100));
}

void Raycaster::render(const Player& player, const Map& map, float playerHealth, bool isHurt) {
    float fov = M_PI / 2.0f;    // Поле зрения 90 градусов

    for (int x = 0; x < screenWidth; ++x) {
        // Вычисляем угол луча относительно направления взгляда игрока
        float rayAngle = player.angle - fov / 2.0f + (static_cast<float>(x) / static_cast<float>(screenWidth)) * fov;

        float rayX = player.x;
        float rayY = player.y;

        // Приращения для движения луча
        float rayCos = static_cast<float>(cos(rayAngle)) * 0.01f;
        float raySin = static_cast<float>(sin(rayAngle)) * 0.01f;

        float distance = 0.0f;
        bool hitWall = false;
        bool isSoft = false;

        while (!hitWall && distance < 20.0f) {
            rayX += rayCos;
            rayY += raySin;
            distance += 0.01f;

            if (map.isWall(rayX, rayY)) {
                hitWall = true;
                int ix = static_cast<int>(rayX);
                int iy = static_cast<int>(rayY);

                if ((ix < 0 && map.softSide == 2) ||
                    (ix >= map.width && map.softSide == 3) ||
                    (iy < 0 && map.softSide == 0) ||
                    (iy >= map.height && map.softSide == 1)) {
                    isSoft = true;
                }
                else if (ix >= 0 && ix < map.width && iy >= 0 && iy < map.height) {
                    isSoft = (map.grid[iy][ix] == 0);
                }
            }
        }

        int wallHeight = hitWall ? static_cast<int>(screenHeight / distance) : 0;
        int wallTop = (screenHeight - wallHeight) / 2;

        sf::Color ceilingColor(30, 30, 30);
        if (playerHealth < 50) {
            ceilingColor.r += static_cast<sf::Uint8>((50 - playerHealth) * 2);
        }

        sf::RectangleShape ceiling(sf::Vector2f(1.0f, static_cast<float>(wallTop)));
        ceiling.setPosition(static_cast<float>(x), 0);
        ceiling.setFillColor(ceilingColor);
        window.draw(ceiling);

        if (hitWall && !isSoft) {
            sf::Color wallColor(100, 100, 100);

            if (playerHealth < 30) {
                wallColor.r += static_cast<sf::Uint8>((30 - playerHealth) * 3);
                wallColor.g -= static_cast<sf::Uint8>((30 - playerHealth) * 2);
                wallColor.b -= static_cast<sf::Uint8>((30 - playerHealth) * 2);
            }

            sf::RectangleShape wall(
                sf::Vector2f(1.0f, static_cast<float>(wallHeight)));
            wall.setPosition(static_cast<float>(x), static_cast<float>(wallTop));
            wall.setFillColor(wallColor);
            window.draw(wall);
        }

        sf::Color floorColor(40, 40, 40);
        if (playerHealth < 30) {
            floorColor.r += static_cast<sf::Uint8>((30 - playerHealth));
        }

        sf::RectangleShape floor(sf::Vector2f(
            1.0f, static_cast<float>(screenHeight - wallTop - wallHeight)));
        floor.setPosition(static_cast<float>(x),
            static_cast<float>(wallTop + wallHeight));
        floor.setFillColor(floorColor);
        window.draw(floor);
    }

    for (const auto& sanit : map.sanitarians) {
        if (sanit.active) {

            float dx = sanit.x - player.x;
            float dy = sanit.y - player.y;

            float distToPlayer = sqrt(dx * dx + dy * dy);
            float playerDirX = cos(player.angle);
            float playerDirY = sin(player.angle);

            float dot = dx * playerDirX + dy * playerDirY;
            float cross = dx * playerDirY - dy * playerDirX;

            float angleToPlayer = atan2(cross, dot);

            if (distToPlayer < 10.0f && fabs(angleToPlayer) < M_PI / 4) {

                float screenX = (angleToPlayer / (M_PI / 4)) * (screenWidth / 2) + (screenWidth / 2);
                float screenY = screenHeight / 2;
                float spriteSize = 100.0f / distToPlayer;

                if (spriteSize > 5.0f && screenX > -100 && screenX < screenWidth + 100) {

                    sf::RectangleShape sanitSprite(sf::Vector2f(spriteSize, spriteSize));
                    sanitSprite.setPosition(screenX - spriteSize / 2, screenY - spriteSize / 2);
                    sanitSprite.setFillColor(sf::Color::White);
                    window.draw(sanitSprite);

                    sf::CircleShape eye1(spriteSize / 6);
                    eye1.setPosition(screenX - spriteSize / 3, screenY - spriteSize / 6);
                    eye1.setFillColor(sf::Color::Red);
                    window.draw(eye1);

                    sf::CircleShape eye2(spriteSize / 6);
                    eye2.setPosition(screenX + spriteSize / 6, screenY - spriteSize / 6);
                    eye2.setFillColor(sf::Color::Red);
                    window.draw(eye2);
                }
            }
        }
    }

    if (isHurt) {
        showHurtEffect = true;
        hurtTimer.restart();
    }

    if (showHurtEffect) {
        float elapsed = hurtTimer.getElapsedTime().asSeconds();
        if (elapsed < 0.3f) {
            float alpha = 100 * (1.0f - (elapsed / 0.3f));
            hurtOverlay.setFillColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(alpha)));
            window.draw(hurtOverlay);
        }
        else {
            showHurtEffect = false;
        }
    }
}