#include "Map.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

Map::Map(int w, int h) : width(w), height(h), softSide(0) {
    grid.resize(height, std::vector<int>(width, 0));  // Инициализация сетки нулями
    srand(static_cast<unsigned int>(time(0)));
}

void Map::generateRandom(int roomCount) {

    for (auto& row : grid) std::fill(row.begin(), row.end(), 0);

    // Случайно выбираем сторону с мягкой стеной (0-верх, 1-низ, 2-лево, 3-право)
    softSide = rand() % 4;

    // Создаем комнату со стенами по краям и одной мягкой стеной
    if (softSide == 0) {
        for (int i = 0; i < width; ++i) grid[0][i] = 0;
        for (int i = 1; i < height - 1; ++i) {
            grid[i][0] = 1;
            grid[i][width - 1] = 1;
        }
        for (int i = 0; i < width; ++i) grid[height - 1][i] = 1;
    }
    else if (softSide == 1) {
        for (int i = 0; i < width; ++i) grid[height - 1][i] = 0;
        for (int i = 1; i < height - 1; ++i) {
            grid[i][0] = 1;
            grid[i][width - 1] = 1;
        }
        for (int i = 0; i < width; ++i) grid[0][i] = 1;
    }
    else if (softSide == 2) {
        for (int i = 0; i < height; ++i) grid[i][0] = 0;
        for (int i = 1; i < width - 1; ++i) {
            grid[0][i] = 1;
            grid[height - 1][i] = 1;
        }
        for (int i = 0; i < height; ++i) grid[i][width - 1] = 1;
    }
    else {
        for (int i = 0; i < height; ++i) grid[i][width - 1] = 0;
        for (int i = 1; i < width - 1; ++i) {
            grid[0][i] = 1;
            grid[height - 1][i] = 1;
        }
        for (int i = 0; i < height; ++i) grid[i][0] = 1;
    }

    // Добавляем случайные внутренние стены для усложнения
    for (int i = 0; i < 3 + roomCount; i++) {
        int wallX = 2 + rand() % (width - 4);
        int wallY = 2 + rand() % (height - 4);
        if (grid[wallY][wallX] == 0) {
            grid[wallY][wallX] = 1;
        }
    }

    sanitarians.clear();

    // Количество санитаров увеличивается с каждой пройденной комнатой
    int sanitCount = 1 + roomCount / 2;
    if (sanitCount > 5) sanitCount = 5;

    for (int i = 0; i < sanitCount; i++) {
        Sanitarian sanit;
        sanit.active = true;
        sanit.speed = 1.5f + roomCount * 0.2f;

        // Размещаем санитара в случайном углу комнаты
        int corner = rand() % 4;
        switch (corner) {
        case 0: sanit.x = 1.5f; sanit.y = 1.5f; break;  // Левый верхний
        case 1: sanit.x = width - 1.5f; sanit.y = 1.5f; break;  // Правый верхний
        case 2: sanit.x = 1.5f; sanit.y = height - 1.5f; break; // Левый нижний
        case 3: sanit.x = width - 1.5f; sanit.y = height - 1.5f; break; // Правый нижний
        }

        sanitarians.push_back(sanit);
    }
}

bool Map::isWall(float x, float y) const {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    if (ix < 0 || ix >= width || iy < 0 || iy >= height) return true;
    return grid[iy][ix] == 1;
}

bool Map::isCell(float x, float y) const {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    return ix >= 0 && ix < width && iy >= 0 && iy < height;
}

void Map::updateSanitarians(float playerX, float playerY, float deltaTime, float& playerHealth, bool gameEnded) {
    if (gameEnded) return;

    for (auto& sanit : sanitarians) {
        if (!sanit.active) continue;

        // Вычисляем вектор направления к игроку
        float dx = playerX - sanit.x;
        float dy = playerY - sanit.y;
        float dist = sqrt(dx * dx + dy * dy);

        if (dist > 0.1f) {

            dx /= dist;
            dy /= dist;

            float newX = sanit.x + dx * sanit.speed * deltaTime;
            float newY = sanit.y + dy * sanit.speed * deltaTime;

            if (!isWall(newX, newY)) {
                sanit.x = newX;
                sanit.y = newY;
            }
        }

        if (dist < 1.0f) {
            static float lastDamageTime = 0;
            float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;

            if (currentTime - lastDamageTime > 0.5f) {
                playerHealth -= 10.0f;
                lastDamageTime = currentTime;
            }
        }
    }
}