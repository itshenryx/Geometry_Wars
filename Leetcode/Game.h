#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "EntityManager.h"

class Game {
  sf::RenderWindow m_window;
  EntityManager m_entities;
  sf::Font m_font;
  sf::Text m_text;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  bool m_paused = false;
  bool m_running = true;
  Vec2 m_mousePos = {0,0};

  std::shared_ptr<Entity> player;

  void init(const std::string& config);
  void setPaused(bool paused);

  void sMovement();
  void sUserInput();
  void sRender();
  void sCollision();
  void sLifespan();
  void sEnemySpawner();

  
  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity, float bulletAngle);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
  void moveSpecial();
  
public:
  Game(const std::string& path);

  void run();
};
