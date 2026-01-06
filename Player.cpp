#define _USE_MATH_DEFINES
#include "Player.h"

#include <cmath>

#include "Map.h"

Player::Player(float startX, float startY)
    : x(startX), y(startY), angle(0.0f), speed(5.0f), baseSpeed(5.0f), hitSoft(false) {
}

void Player::moveForward(float deltaTime, const Map& map) {

    float newX = x + static_cast<float>(cos(angle)) * speed * deltaTime;
    float newY = y + static_cast<float>(sin(angle)) * speed * deltaTime;

    int ix = static_cast<int>(newX);
    int iy = static_cast<int>(newY);

    // Проверяем столкновение со стеной
    if (map.isWall(newX, newY)) {
        // Если это мягкая стена (выход из комнаты), проходим через нее
        if ((ix < 0 && map.softSide == 2) ||
            (ix >= map.width && map.softSide == 3) ||
            (iy < 0 && map.softSide == 0) ||
            (iy >= map.height && map.softSide == 1)) {
            x = newX;
            y = newY;
            hitSoft = true;
        }
    }
    else {

        x = newX;
        y = newY;
    }
}

void Player::moveBackward(float deltaTime, const Map& map) {
    float newX = x - static_cast<float>(cos(angle)) * speed * deltaTime;
    float newY = y - static_cast<float>(sin(angle)) * speed * deltaTime;

    int ix = static_cast<int>(newX);
    int iy = static_cast<int>(newY);

    if (map.isWall(newX, newY)) {
        if ((ix < 0 && map.softSide == 2) ||
            (ix >= map.width && map.softSide == 3) ||
            (iy < 0 && map.softSide == 0) ||
            (iy >= map.height && map.softSide == 1)) {
            x = newX;
            y = newY;
            hitSoft = true;
        }
    }
    else {
        x = newX;
        y = newY;
    }
}

void Player::strafeLeft(float deltaTime, const Map& map) {
    float newX = x + static_cast<float>(cos(angle - M_PI / 2)) * speed * deltaTime;
    float newY = y + static_cast<float>(sin(angle - M_PI / 2)) * speed * deltaTime;

    int ix = static_cast<int>(newX);
    int iy = static_cast<int>(newY);

    if (map.isWall(newX, newY)) {
        if ((ix < 0 && map.softSide == 2) ||
            (ix >= map.width && map.softSide == 3) ||
            (iy < 0 && map.softSide == 0) ||
            (iy >= map.height && map.softSide == 1)) {
            x = newX;
            y = newY;
            hitSoft = true;
        }
    }
    else {
        x = newX;
        y = newY;
    }
}

void Player::strafeRight(float deltaTime, const Map& map) {
    float newX = x + static_cast<float>(cos(angle + M_PI / 2)) * speed * deltaTime;
    float newY = y + static_cast<float>(sin(angle + M_PI / 2)) * speed * deltaTime;

    int ix = static_cast<int>(newX);
    int iy = static_cast<int>(newY);

    if (map.isWall(newX, newY)) {
        if ((ix < 0 && map.softSide == 2) ||
            (ix >= map.width && map.softSide == 3) ||
            (iy < 0 && map.softSide == 0) ||
            (iy >= map.height && map.softSide == 1)) {
            x = newX;
            y = newY;
            hitSoft = true;
        }
    }
    else {
        x = newX;
        y = newY;
    }
}

// Движение в произвольном направлении
void Player::moveInDirection(float dirAngle, float deltaTime, const Map& map) {
    float moveDistance = speed * deltaTime;
    float newX = x + static_cast<float>(cos(dirAngle)) * moveDistance;
    float newY = y + static_cast<float>(sin(dirAngle)) * moveDistance;

    int ix = static_cast<int>(newX);
    int iy = static_cast<int>(newY);

    if (map.isWall(newX, newY)) {
        if ((ix < 0 && map.softSide == 2) ||
            (ix >= map.width && map.softSide == 3) ||
            (iy < 0 && map.softSide == 0) ||
            (iy >= map.height && map.softSide == 1)) {
            x = newX;
            y = newY;
            hitSoft = true;
        }
    }
    else {
        x = newX;
        y = newY;
    }
}

void Player::rotate(float deltaAngle) { angle += deltaAngle; }