/*
 * Class that is responsible for the game engine
 */

#ifndef HELLOSFML_GAME_H
#define HELLOSFML_GAME_H

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Game {
private:
    // Variables
    sf::RenderWindow *m_window{nullptr};
    sf::VideoMode m_videoMode;
    sf::Event m_ev;

    // Mouse Position
    sf::Vector2i m_mousePosWindow;
    sf::Vector2f m_mousePosView;

    // Game logic
    int m_points{};
    float m_enemySpawnTimer{};
    float m_enemySpawnTimerMax{};
    int m_maxEnemies{};

    // Private functions
    void initVars();
    void initWindow();
    void initEnemies();

    // Game objects
    std::vector<sf::RectangleShape> m_enemies;
    sf::RectangleShape m_enemy;

public:
    // Constructors / Destructors
    Game();
    virtual ~Game();

    // Functions
    void update();
    void render();
    void renderEnemies();
    void updateMousePositions();
    void pollEvents();
    void spawnEnemy();
    void updateEnemies();
    // Getters
    const bool isRunning() const;
};


#endif //HELLOSFML_GAME_H
