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
  m_points = 0;
  m_enemySpawnTimerMax =  1000.f;
  m_enemySpawnTimer = m_enemySpawnTimerMax;
  m_maxEnemies = 5;
}

void Game::initWindow() {
  m_videoMode.height = 1200;
  m_videoMode.width = 1600;
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

void Game::spawnEnemy() {
  m_enemy.setPosition(
          static_cast<float>(rand()%static_cast<int>(m_window->getSize().x - m_enemy.getSize().x)),
          0.f
          );

  m_enemy.setFillColor(sf::Color::Green);
  m_enemies.push_back(m_enemy);
}

void Game::renderEnemies() {
  for (auto &e : m_enemies) {
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

    // Check if clicked on, delete it, then continue the loop
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      if (m_enemies[i].getGlobalBounds().contains(m_mousePosView.x, m_mousePosView.y)) {
        m_enemies.erase(m_enemies.begin() + i);
        continue;
      }
    }

    // Check if the enemy is outside the window
    if (m_enemies[i].getPosition().y > m_window->getSize().y) m_enemies.erase(m_enemies.begin() + i);
  }


}

// Getters
const bool Game::isRunning() const {
  return m_window->isOpen();
}

// public functions
void Game::update() {
  pollEvents();
  updateMousePositions();
  updateEnemies();
}

void Game::render() {
  m_window->clear();
  // Draw game
  renderEnemies();
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
        break;
    }
  }
}

void Game::updateMousePositions() {
  m_mousePosWindow = sf::Mouse::getPosition(*m_window);
  m_mousePosView = m_window->mapPixelToCoords(m_mousePosWindow);
}


