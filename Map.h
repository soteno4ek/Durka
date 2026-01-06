#ifndef MAP_H
#define MAP_H

#include <vector>

struct Sanitarian {
    float x, y;
    float speed;
    bool active;
};

class Map {
public:
    int width, height;
    int softSide;
    std::vector<std::vector<int>> grid; // 2D сетка стен (0 - проходимо, 1 - стена)
    std::vector<Sanitarian> sanitarians;    // Список санитаров на уровне

    Map(int w, int h);
    void generateRandom(int roomCount);
    bool isWall(float x, float y) const;    // Проверка на стену
    bool isCell(float x, float y) const;    // Проверка в пределах карты
    void updateSanitarians(float playerX, float playerY, float deltaTime, float& playerHealth, bool gameEnded);
};

#endif