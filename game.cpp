#include "game.h"

// Constructor / Destructor
Game::Game() {
  initVars();
  initWindow();
  initEnemies();
}

Game::~Game() {
  delete m_window;
}

// private functions
void Game::initVars() {
  m_endGame = false;
  m_points = 0;
  m_enemySpawnTimerMax = 1000.f;
  m_enemySpawnTimer = m_enemySpawnTimerMax;
  m_maxEnemies = 5;
  m_isMouseHeld = false;
  m_health = STARTING_HEALTH;

  m_titleScreen = true;

  if (!m_font.loadFromFile("../Minecraft.ttf")) {
    std::cerr << "Error: Game()::initVars() - Failed to load font\n";
  } else {
    // Title screen text
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(40);
    m_titleText.setFillColor(sf::Color::Green);

    m_pressToStartText.setFont(m_font);
    m_pressToStartText.setCharacterSize(25);
    m_pressToStartText.setFillColor(sf::Color::White);

    // Game text
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(25);
    m_scoreText.setFillColor(sf::Color::White);

    m_healthText.setFont(m_font);
    m_healthText.setCharacterSize(25);
    m_healthText.setFillColor(sf::Color::Red);

    // Game over screen text
    m_gameOverText.setFont(m_font);
    m_gameOverText.setCharacterSize(30);
    m_gameOverText.setFillColor(sf::Color::Red);

    m_pressToRestartText.setFont(m_font);
    m_pressToRestartText.setCharacterSize(20);
    m_pressToRestartText.setFillColor(sf::Color::White);
  }
}

void Game::initWindow() {
  m_videoMode.height = 800;
  m_videoMode.width = 1000;
  m_window = new sf::RenderWindow(m_videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
  m_window->setFramerateLimit(60);
}

void Game::initEnemies() {
  m_enemy.setPosition(10.f, 10.f);
  m_enemy.setSize(sf::Vector2f(200.f, 200.f));
  m_enemy.setScale(sf::Vector2f(0.5, 0.5));
  m_enemy.setFillColor(sf::Color::Cyan);
  m_enemy.setOutlineColor(sf::Color::White);
  m_enemy.setOutlineThickness(1.f);

}

void Game::renderText() {
  // Render the title screen
  if (m_titleScreen) {
    m_titleText.setString("Falling bricks game");

    float titleY{m_window->getSize().y / 2.f - (m_titleText.getLocalBounds().height / 2.f)};

    m_titleText.setPosition(m_window->getSize().x / 2.f - (m_titleText.getLocalBounds().width / 2.f) , titleY);

    m_pressToStartText.setString("Press \"S\" to start!");
    m_pressToStartText.setPosition(m_window->getSize().x / 2.f - (m_pressToStartText.getLocalBounds().width / 2.f), titleY + m_pressToStartText.getLocalBounds().height + 30);

    m_window->draw(m_titleText);
    m_window->draw(m_pressToStartText);
  }

  // render the score and health while the game is running
  if (!isEndGame() && !m_titleScreen) {
    m_scoreText.setString("Points: " + std::to_string(m_points));
    float scoreTextY{m_window->getSize().y - 40.f};
    m_scoreText.setPosition(10, scoreTextY);

    m_healthText.setString("Health: " + std::to_string(m_health));
    m_healthText.setPosition(10, scoreTextY - 30);

    m_window->draw(m_healthText);
    m_window->draw(m_scoreText);
  }

  // render the game over screen
  if (isEndGame() && !m_titleScreen) {
    m_gameOverText.setString("GAME OVER!");

    float gameOverY = m_window->getSize().y / 2.f - (m_gameOverText.getLocalBounds().height / 2.f);

    m_gameOverText.setPosition(m_window->getSize().x / 2.f - (m_gameOverText.getLocalBounds().width / 2.f), gameOverY);
    m_window->draw(m_gameOverText);

    m_pressToRestartText.setString("Press \"R\" to restart, or \"T\" for the title screen");
    m_pressToRestartText.setPosition(m_window->getSize().x / 2.f - (m_pressToRestartText.getLocalBounds().width / 2.f), gameOverY + m_pressToRestartText.getLocalBounds().height + 20.f);

    m_window->draw(m_pressToRestartText);
  }

}

void Game::spawnEnemy() {
  m_enemy.setPosition(
          static_cast<float>(rand() % static_cast<int>(m_window->getSize().x - m_enemy.getSize().x)),
          0.f
  );

  // Randomize Enemy type
  int type{rand()%5};
  switch (type) {
    case 0:
      m_enemy.setScale(0.25, 0.25);
      m_enemy.setFillColor(sf::Color::Magenta);
      break;
    case 1:
      m_enemy.setFillColor(sf::Color::Blue);
      m_enemy.setScale(0.30, 0.30);
      break;
    case 2:
      m_enemy.setFillColor(sf::Color::Cyan);
      m_enemy.setScale(0.5, 0.5);
      break;
    case 3:
      m_enemy.setFillColor(sf::Color::White);
      m_enemy.setScale(0.75, 0.75);
      break;
    case 4:
      m_enemy.setFillColor(sf::Color::Green);
      m_enemy.setScale(1.f, 1.f);
      break;
  }
  m_enemies.push_back(m_enemy);
}

void Game::renderEnemies() {
  for (auto &e: m_enemies) {
    m_window->draw(e);
  }
}

void Game::updateEnemies() {
  // Updating the enemy spawning timer
  if (m_enemies.size() < m_maxEnemies) {
    if (m_enemySpawnTimer >= m_enemySpawnTimerMax) {
      spawnEnemy();
      m_enemySpawnTimer = 0.f;
    } else {
      m_enemySpawnTimer += 70.f;
    }
  }

  // Move the enemies
  for (int i{0}; i < m_enemies.size(); ++i) {
    m_enemies[i].move(0.f, 5.f);


    // Check if the enemy is outside the window
    if (m_enemies[i].getPosition().y > m_window->getSize().y) {
      m_enemies.erase(m_enemies.begin() + i);
      m_health -= 1;
      std::cout << "Health: " << m_health << '\n';
    }
  }

  // Check if clicked on, delete it, then continue the loop
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (!m_isMouseHeld) {
      m_isMouseHeld = true;
      for (size_t i = 0; i < m_enemies.size(); ++i) {
        if (m_enemies[i].getGlobalBounds().contains(m_mousePosView.x, m_mousePosView.y)) {
          m_enemies.erase(m_enemies.begin() + i);
          m_points += 1;

          break;
        }
      }
    }
  } else {
    m_isMouseHeld = false;
  }

}

void Game::clearEnemies() {
  m_enemies.erase(m_enemies.begin(), m_enemies.end());
}

// Getters
const bool Game::isRunning() const {
  return m_window->isOpen();
}

const bool Game::isEndGame() const {
  return m_health <= 0;
}

// public functions

void Game::reset() {
  m_endGame = false;
  m_health = STARTING_HEALTH;
  m_points = 0;
}
void Game::update() {
  pollEvents();
  if (m_endGame == false) {
    if (!m_titleScreen) {
      updateMousePositions();
      updateEnemies();
    }
  }

  if (isEndGame()) {
    m_endGame = true;
    clearEnemies();
  }
}

void Game::render() {
  m_window->clear();
  // Draw game
  renderEnemies();
  renderText();
  m_window->display();
}

void Game::pollEvents() {
  while (m_window->pollEvent(m_ev)) {
    switch (m_ev.type) {
      case sf::Event::Closed:
        m_window->close();
        break;
      case sf::Event::KeyPressed:
        if (m_ev.key.code == sf::Keyboard::Escape) m_window->close();

        else if ((m_ev.key.code == sf::Keyboard::R) && isEndGame()) {
          m_endGame = false;
          reset();
        }

        else if ((m_ev.key.code == sf::Keyboard::S) && m_titleScreen) {
          m_titleScreen = false;
        }

        else if ((m_ev.key.code == sf::Keyboard::T) && isEndGame()) {
          m_titleScreen = true;
          reset();
        }
        break;
    }
  }
}

void Game::updateMousePositions() {
  m_mousePosWindow = sf::Mouse::getPosition(*m_window);
  m_mousePosView = m_window->mapPixelToCoords(m_mousePosWindow);
}


