#include "Game.h"

#include "Map.h"
#include "Player.h"
#include "Raycaster.h"
#include <sstream>
#include <iostream>
#include <iomanip>

Game::Game(sf::RenderWindow& window)
    : window(window),
    roomCount(0),
    gameWon(false),
    gameLost(false),
    isPaused(false),
    playerHealth(100.0f),
    maxHealth(100.0f),
    playerIsHurt(false) {

    // Инициализация центра экрана для управления мышью
    centerMouse = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(centerMouse, window);

    // Создание объектов игры
    player = new Player(1.5f, 1.5f);
    map = new Map(10, 10);
    raycaster = new Raycaster(window, *map);

    map->generateRandom(roomCount);

    // Загрузка шрифта с проверкой
    bool fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/times.ttf");
    }
    if (!fontLoaded) {
        std::cout << "Warning: Could not load font files\n";
        std::cout << "Game will continue with default font\n";
    }

    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(10, 10);

    roomText.setFont(font);
    roomText.setCharacterSize(20);
    roomText.setFillColor(sf::Color::White);
    roomText.setPosition(10, 40);

    healthBarBg.setSize(sf::Vector2f(202, 22));
    healthBarBg.setPosition(200, 12);
    healthBarBg.setFillColor(sf::Color(50, 50, 50));
    healthBarBg.setOutlineThickness(1);
    healthBarBg.setOutlineColor(sf::Color::White);

    healthBar.setSize(sf::Vector2f(200, 20));
    healthBar.setPosition(201, 13);
    healthBar.setFillColor(sf::Color::Green);
}

Game::~Game() {
    delete player;
    delete map;
    delete raycaster;
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleInput(deltaTime);
        if (!isPaused) {
            update(deltaTime);
        }
        render();
    }
}

void Game::update(float deltaTime) {
    bool gameEnded = gameWon || gameLost;

    map->updateSanitarians(player->x, player->y, deltaTime, playerHealth, gameEnded);

    static float lastHealth = playerHealth;
    if (playerHealth < lastHealth) {
        playerIsHurt = true;
    }
    lastHealth = playerHealth;

    if (playerHealth <= 0 && !gameEnded) {
        gameLost = true;
        player->speed = 0;
        window.setMouseCursorVisible(true);
    }

    // Проверка выхода из комнаты (через мягкую стену)
    if (player->hitSoft && !gameEnded) {
        player->hitSoft = false;
        roomCount++;
        if (roomCount >= 10) {
            gameWon = true;
            window.setMouseCursorVisible(true);
        }
        else {
            generateNewRoom();
            playerHealth += 5.0f;
            if (playerHealth > maxHealth) playerHealth = maxHealth;
        }
    }

    // Регенерация здоровья
    if (playerHealth < maxHealth && !gameEnded) {
        playerHealth += 0.3f * deltaTime;
        if (playerHealth > maxHealth) playerHealth = maxHealth;
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    if (!gameWon && !gameLost) {
        if (!isPaused) {
            raycaster->render(*player, *map, playerHealth, playerIsHurt);
            playerIsHurt = false;
            renderHUD();
        }
        else {
            sf::RectangleShape menuBg(sf::Vector2f(320, 220));
            menuBg.setPosition(240, 190);
            menuBg.setFillColor(sf::Color(20, 20, 20, 220));
            menuBg.setOutlineThickness(5);
            menuBg.setOutlineColor(sf::Color(100, 100, 100));
            window.draw(menuBg);

            sf::Text title("PAUSE", font, 30);
            title.setPosition(350, 200);
            title.setFillColor(sf::Color::White);
            window.draw(title);

            sf::RectangleShape continueRect(sf::Vector2f(200, 50));
            continueRect.setPosition(300, 250);
            continueRect.setFillColor(sf::Color(70, 70, 70));
            continueRect.setOutlineThickness(2);
            continueRect.setOutlineColor(sf::Color::White);
            window.draw(continueRect);

            sf::Text continueText("Resume", font, 20);
            continueText.setPosition(310, 265);
            continueText.setFillColor(sf::Color::White);
            window.draw(continueText);

            sf::RectangleShape exitRect(sf::Vector2f(200, 50));
            exitRect.setPosition(300, 320);
            exitRect.setFillColor(sf::Color(70, 70, 70));
            exitRect.setOutlineThickness(2);
            exitRect.setOutlineColor(sf::Color::White);
            window.draw(exitRect);

            sf::Text exitText("Exit Game", font, 20);
            exitText.setPosition(320, 335);
            exitText.setFillColor(sf::Color::White);
            window.draw(exitText);
        }
    }

    if (gameWon) {
        sf::RectangleShape winBg(sf::Vector2f(320, 220));
        winBg.setPosition(240, 190);
        winBg.setFillColor(sf::Color(20, 20, 20, 220));
        winBg.setOutlineThickness(5);
        winBg.setOutlineColor(sf::Color(100, 100, 100));
        window.draw(winBg);

        sf::Text winText("YOU ESCAPED\nFROM THE DURKA!", font, 30);
        winText.setPosition(250, 220);
        winText.setFillColor(sf::Color::White);
        winText.setLineSpacing(1.5f);
        window.draw(winText);


        sf::Text winTextClick("(Click to close the game)", font, 20);
        winTextClick.setPosition(300, 350);
        winTextClick.setFillColor(sf::Color::White);
        winTextClick.setLineSpacing(1.5f);
        window.draw(winTextClick);

    }

    if (gameLost) {
        sf::RectangleShape loseBg(sf::Vector2f(320, 220));
        loseBg.setPosition(240, 190);
        loseBg.setFillColor(sf::Color(20, 20, 20, 220));
        loseBg.setOutlineThickness(5);
        loseBg.setOutlineColor(sf::Color(100, 100, 100));
        window.draw(loseBg);

        sf::Text loseText("SANITARIANS\nCAUGHT YOU!", font, 30);
        loseText.setPosition(250, 220);
        loseText.setFillColor(sf::Color::White);
        loseText.setLineSpacing(1.5f);
        window.draw(loseText);

        sf::Text restartText("Press R to restart", font, 20);
        restartText.setPosition(270, 320);
        restartText.setFillColor(sf::Color::White);
        window.draw(restartText);
    }

    window.display();
}

void Game::renderHUD() {

    float healthPercent = playerHealth / maxHealth;
    healthBar.setSize(sf::Vector2f(200 * healthPercent, 20));

    // Изменение цвета полоски в зависимости от здоровья
    if (healthPercent > 0.5f) {
        healthBar.setFillColor(sf::Color::Green);
    }
    else if (healthPercent > 0.25f) {
        healthBar.setFillColor(sf::Color::Yellow);
    }
    else {
        healthBar.setFillColor(sf::Color::Red);
    }

    std::stringstream healthSS;
    healthSS << "Health: " << std::fixed << std::setprecision(0) << playerHealth << "/" << maxHealth;
    healthText.setString(healthSS.str());

    std::stringstream roomSS;
    roomSS << "Room: " << roomCount + 1 << "/10";
    roomText.setString(roomSS.str());

    window.draw(healthBarBg);
    window.draw(healthBar);
    window.draw(healthText);
    window.draw(roomText);

    // Предупреждение при низком здоровье (мигающий текст)
    if (playerHealth < 30 && !gameWon && !gameLost) {
        sf::Text warning("WARNING: LOW HEALTH!", font, 18);
        warning.setPosition(300, 560);
        warning.setFillColor(sf::Color::Red);

        static float blinkTimer = 0;
        blinkTimer += 0.05f;
        if (static_cast<int>(blinkTimer) % 2 == 0) {
            window.draw(warning);
        }
    }
}

// Обработка ввода пользователя
void Game::handleInput(float deltaTime) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (gameLost && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                resetGame();
            }
        }

        if (gameWon && event.type == sf::Event::MouseButtonPressed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (gameWon || gameLost) {
                    window.close();
                }
                else if (isPaused) {
                    isPaused = false;
                    window.setMouseCursorVisible(false);
                    sf::Mouse::setPosition(centerMouse, window);
                }
                else {
                    isPaused = true;
                    window.setMouseCursorVisible(true);
                }
            }
            if (isPaused) {
                if (event.key.code == sf::Keyboard::Enter) {
                    isPaused = false;
                    window.setMouseCursorVisible(false);
                    sf::Mouse::setPosition(centerMouse, window);
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close();
                }
            }
        }

        if (isPaused && event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::RectangleShape continueRect(sf::Vector2f(200, 50));
            continueRect.setPosition(300, 250);
            sf::RectangleShape exitRect(sf::Vector2f(200, 50));
            exitRect.setPosition(300, 320);
            if (continueRect.getGlobalBounds().contains(
                static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                isPaused = false;
                window.setMouseCursorVisible(false);
                sf::Mouse::setPosition(centerMouse, window);
            }
            else if (exitRect.getGlobalBounds().contains(
                static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y))) {
                window.close();
            }
        }
    }

    if (!isPaused && !gameWon && !gameLost) {

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float deltaX = static_cast<float>(mousePos.x - centerMouse.x);
        player->rotate(deltaX * 0.005f);
        sf::Mouse::setPosition(centerMouse, window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player->moveForward(deltaTime, *map);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player->moveBackward(deltaTime, *map);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player->strafeLeft(deltaTime, *map);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player->strafeRight(deltaTime, *map);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            player->speed = 8.0f;
        }
        else {
            player->speed = 5.0f;
        }
    }
}

void Game::generateNewRoom() {
    map->generateRandom(roomCount);
    // Возвращаем игрока в центр новой комнаты
    player->x = map->width / 2.0f;
    player->y = map->height / 2.0f;
    player->angle = 0.0f;
}

void Game::resetGame() {
    roomCount = 0;
    gameWon = false;
    gameLost = false;
    playerHealth = maxHealth;

    delete player;
    delete map;
    delete raycaster;

    player = new Player(1.5f, 1.5f);
    map = new Map(10, 10);
    raycaster = new Raycaster(window, *map);

    map->generateRandom(roomCount);

    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(centerMouse, window);
}