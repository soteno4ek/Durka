#ifndef PLAYER_H
#define PLAYER_H

class Map;

class Player {
public:
    float x, y, angle;  // Позиция и направление взгляда
    float speed;
    float baseSpeed;
    bool hitSoft;   // Флаг столкновения с мягкой стеной

    Player(float startX, float startY);
    void moveForward(float deltaTime, const Map& map);
    void moveBackward(float deltaTime, const Map& map);
    void strafeLeft(float deltaTime, const Map& map);
    void strafeRight(float deltaTime, const Map& map);
    void moveInDirection(float dirAngle, float deltaTime, const Map& map);
    void rotate(float deltaAngle);
};

#endif