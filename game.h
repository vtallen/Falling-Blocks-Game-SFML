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

    // Resources
    sf::Font m_font;

    // End game text
    sf::Text m_gameOverText;
    sf::Text m_pressToRestartText;

    // Title Screen text
    sf::Text m_titleText;
    sf::Text m_pressToStartText;

    // Score and health text
    sf::Text m_scoreText;
    sf::Text m_healthText;

    // Game logic
    int m_points{};

    static constexpr int STARTING_HEALTH{10};

    int m_health{};
    float m_enemySpawnTimer{};
    float m_enemySpawnTimerMax{};
    int m_maxEnemies{};
    bool m_isMouseHeld{};

    bool m_endGame{};
    bool m_titleScreen{};

    // Private functions
    void initVars();
    void initWindow();
    void initEnemies();
    void renderText();

    void clearEnemies();

    // Game objects
    std::vector<sf::RectangleShape> m_enemies;
    sf::RectangleShape m_enemy;

public:
    // Constructors / Destructors
    Game();
    virtual ~Game();

    // Functions
    void reset();
    void update();
    void render();
    void renderEnemies();
    void updateMousePositions();
    void pollEvents();
    void spawnEnemy();
    void updateEnemies();
    // Getters
    const bool isRunning() const;
    const bool isEndGame() const;
};


#endif //HELLOSFML_GAME_H
