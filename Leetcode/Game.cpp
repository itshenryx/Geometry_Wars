#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game(const std::string& path) {
  init(path);
}

void Game::init(const std::string& path) {
  m_window.create(sf::VideoMode(1980, 1080), "Game 2 in 4 hours", sf::Style::Fullscreen);
  m_window.setFramerateLimit(144);

  spawnPlayer();
}

void Game::run() {
  while (m_running) {
    m_entities.update();

    sUserInput();
    sCollision();
    sEnemySpawner();
    sLifespan();
    sMovement();

    sRender();

    m_currentFrame++;
  }
}

void Game::setPaused(bool paused) {
  m_paused = paused;
}

void Game::spawnPlayer() {
  auto entity = m_entities.addEntity("player");

  float px = m_window.getSize().x / 2;
  float py = m_window.getSize().y / 2;

  entity->cTransform = std::make_shared<CTransform>(Vec2(px, py),
                                                    Vec2(0, 0), 2);
  entity->cShape = std::make_shared<CShape>(20.0f, 8, 0, 0, 0);

  //Temp colors and details for shape
  entity->cShape->shape.setOutlineColor(sf::Color(255, 0, 0));
  entity->cShape->shape.setOutlineThickness(2);

  entity->cInput = std::make_shared<CInput>();

  entity->cCollision = std::make_shared<CCollision>(20);

  entity->cLazer = std::make_shared<CLazer>(m_window.getSize().x);
  player = entity;
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos) {
  auto bullet = m_entities.addEntity("bullet");

  Vec2 velocity = mousePos - entity->cTransform->pos;
  velocity = velocity.normalize() * 8.8f;

  bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, 0);

  bullet->cShape = std::make_shared<CShape>(10, 20, 255, 255, 255);

  bullet->cLifespan = std::make_shared<CLifespan>(120);

  bullet->cCollision = std::make_shared<CCollision>(10);
}

void Game::spawnEnemy() {
  auto entity = m_entities.addEntity("enemy");

  //Size
  float radius = 20 + rand() % 40,
        diameter = radius * 2;

  //Position
  float ex = radius + rand() % (int)(m_window.getSize().x - diameter),
        ey = radius + rand() % (int)(m_window.getSize().y - diameter);

  //Colors
  float r = rand() % 255,
        g = rand() % 255,
        b = rand() % 255;

  //Veloctiy
  float vx = rand() % 3 - 1.5,
        vy = rand() % 3 - 1.5;

  entity->cTransform = std::make_shared<CTransform>(Vec2{ex, ey}, Vec2{vx, vy}, 0);

  entity->cShape = std::make_shared<CShape>(radius, 3 + static_cast<int>(radius) % 6, r, g, b);
  entity->cShape->shape.setOutlineColor(sf::Color(255, 255, 255));
  entity->cShape->shape.setOutlineThickness(2);

  entity->cCollision = std::make_shared<CCollision>(radius);
  m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> parentEntity, float bulletAngle) {
  float radius = parentEntity->cShape->shape.getRadius() / 2;

  if (radius < 10) return;

  float coneAngle = 0.5 * parentEntity->cShape->sides;
  float angle = coneAngle / parentEntity->cShape->sides;
  float speed = 5.8;

  for (int i = 0; i < parentEntity->cShape->sides; i++) {
    auto entity = m_entities.addEntity("enemy");
    entity->cTransform = std::make_shared<CTransform>(Vec2{
                                                        parentEntity->cTransform->pos.x,
                                                        parentEntity->cTransform->pos.y
                                                      },
                                                      Vec2{
                                                        speed * std::cos(
                                                          bulletAngle + coneAngle / 2 - angle * i),
                                                        speed * std::sin(
                                                          bulletAngle + coneAngle / 2 - angle * i)
                                                      },
                                                      0);

    entity->cShape = std::make_shared<CShape>(radius, parentEntity->cShape->sides,
                                              parentEntity->cShape->color);

    entity->cCollision = std::make_shared<CCollision>(radius);

    entity->cLifespan = std::make_shared<CLifespan>(120);
  }
}

void Game::moveSpecial() {
  float opposite = player->cTransform->pos.y - m_mousePos.y;
  float adjacent = -m_mousePos.x + player->cTransform->pos.x;
  float angle = std::atan2(-opposite, -adjacent);

  player->cLazer->m = std::tan(angle);
  player->cLazer->c = player->cTransform->pos.y - std::tan(angle) * player->cTransform->pos.x;

  player->cLazer->py = opposite < 0;
  player->cLazer->px = adjacent < 0;

  player->cLazer->angle = angle * 180 / 3.14;
}

void Game::sRender() {
  m_window.clear();

  if (player->cInput->special) {
    player->cLazer->shape.setPosition(player->cTransform->pos.x,
                                      player->cTransform->pos.y);
    player->cLazer->shape.setRotation(player->cLazer->angle);
    m_window.draw(player->cLazer->shape);
  }

  for (auto& e : m_entities.getEntities()) {
    e->cShape->shape.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
    e->cShape->shape.setRotation(e->cTransform->angle);

    if (e->cLifespan) e->cShape->shape.setFillColor(e->cShape->color);

    m_window.draw(e->cShape->shape);
  }

  m_window.display();
}

void Game::sUserInput() {
  sf::Event event;

  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_window.close();
      exit(0);
    }

    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        spawnBullet(player, Vec2{(float)event.mouseButton.x, (float)event.mouseButton.y});
      }
      if (event.mouseButton.button == sf::Mouse::Right) {
        player->cInput->special = 1;
        m_mousePos.x = event.mouseButton.x;
        m_mousePos.y = event.mouseButton.y;
      }
    }

    if (player->cInput->special && event.type == sf::Event::MouseMoved) {
      m_mousePos.x = event.mouseMove.x;
      m_mousePos.y = event.mouseMove.y;
    }

    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Right) {
        player->cInput->special = 0;
      }
    }


    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.scancode) {
      case sf::Keyboard::Scan::W:
        {
          player->cInput->up = true;
          break;
        }
      case sf::Keyboard::Scan::S:
        {
          player->cInput->down = true;
          break;
        }
      case sf::Keyboard::Scan::D:
        {
          player->cInput->right = true;
          break;
        }
      case sf::Keyboard::Scan::A:
        {
          player->cInput->left = true;
          break;
        }
      default: break;
      }
    }
    if (event.type == sf::Event::KeyReleased) {
      switch (event.key.scancode) {
      case sf::Keyboard::Scan::W:
        {
          player->cInput->up = false;
          break;
        }
      case sf::Keyboard::Scan::S:
        {
          player->cInput->down = false;
          break;
        }
      case sf::Keyboard::Scan::D:
        {
          player->cInput->right = false;
          break;
        }
      case sf::Keyboard::Scan::A:
        {
          player->cInput->left = false;
          break;
        }
      case sf::Keyboard::Scan::Escape:
        {
          m_window.close();
          exit(0);
        }
      default: break;
      }
    }
  }
}

void Game::sEnemySpawner() {
  if (m_currentFrame - m_lastEnemySpawnTime == 120) {
    spawnEnemy();
  }
}

void Game::sLifespan() {
  for (auto& e : m_entities.getEntities()) {
    if (e->cLifespan) {
      e->cShape->color.a = (e->cLifespan->remaining / e->cLifespan->total) * 255;
      if (e->cLifespan->remaining-- == 0) {
        e->destroy();
      }
    }
  }
}

void Game::sCollision() {
  for (auto& bullet : m_entities.getEntities("bullet")) {
    for (auto& enemy : m_entities.getEntities("enemy")) {
      if (enemy->cLifespan && enemy->cLifespan->remaining / enemy->cLifespan->total > 0.95) { continue; }
      
      Vec2 distanceVector = enemy->cTransform->pos - bullet->cTransform->pos;
      float length = enemy->cCollision->radius + bullet->cCollision->radius;

      if ((distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y) <
        length * length) {
        float angle = std::atan2(bullet->cTransform->velocity.y, bullet->cTransform->velocity.x);
        spawnSmallEnemies(enemy, angle);

        bullet->destroy();
        enemy->destroy();
      }
    }
  }

  for (auto& enemy : m_entities.getEntities("enemy")) {
    if (player->cInput->special) {

      if (enemy->cLifespan && enemy->cLifespan->remaining / enemy->cLifespan->total > 0.95) { continue; }
      
      if (player->cLazer->px != (enemy->cTransform->pos.x - player->cTransform->pos.x >= 0)) { continue; }
      if (player->cLazer->py != (enemy->cTransform->pos.y - player->cTransform->pos.y >= 0)) { continue; }

      float a = -player->cLazer->m,
            b = 1,
            c = -player->cLazer->c;

      float distance =
        abs(a * enemy->cTransform->pos.x + b * enemy->cTransform->pos.y + c) / sqrt(a * a + b * b);

      if (distance < enemy->cCollision->radius) {
        spawnSmallEnemies(enemy, player->cLazer->angle * 3.14 / 180);
        enemy->destroy();
        continue;
      }
    }

    Vec2 distanceVector = enemy->cTransform->pos - player->cTransform->pos;

    float eColRadius = enemy->cCollision->radius;
    float length = eColRadius + player->cCollision->radius;

    if ((distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y) <
      length * length) {
      player->destroy();
      enemy->destroy();
      spawnPlayer();
      continue;
    }

    if (enemy->cTransform->pos.x < eColRadius ||
      enemy->cTransform->pos.x > m_window.getSize().x - eColRadius) {
      enemy->cTransform->velocity.x *= -1.05;
    }
    if (enemy->cTransform->pos.y < eColRadius ||
      enemy->cTransform->pos.y > m_window.getSize().y - eColRadius) {
      enemy->cTransform->velocity.y *= -1.05;
    }
  }
}

void Game::sMovement() {
  for (auto& smallEnemy : m_entities.getEntities("smallEnemy")) {
    smallEnemy->cTransform->pos += smallEnemy->cTransform->velocity;
    smallEnemy->cTransform->angle += 2;
  }

  for (auto& bullet : m_entities.getEntities("bullet")) {
    bullet->cTransform->pos += bullet->cTransform->velocity;
  }

  for (auto& enemy : m_entities.getEntities("enemy")) {
    enemy->cTransform->pos += enemy->cTransform->velocity;
    enemy->cTransform->velocity %= 2;
    enemy->cTransform->angle += 2;
  }

  player->cTransform->velocity = {0, 0};

  if (player->cInput->up) {
    if (player->cTransform->pos.y > player->cCollision->radius) {
      player->cTransform->velocity.y += -5;
    }
  }
  if (player->cInput->down) {
    if (player->cTransform->pos.y < m_window.getSize().y - player->cCollision->radius) {
      player->cTransform->velocity.y += 5;
    }
  }
  if (player->cInput->right) {
    if (player->cTransform->pos.x < m_window.getSize().x - player->cCollision->radius) {
      player->cTransform->velocity.x += 5;
    }
  }
  if (player->cInput->left) {
    if (player->cTransform->pos.x > player->cCollision->radius) {
      player->cTransform->velocity.x += -5;
    }
  }

  if (player->cInput->special) {
    moveSpecial();
  }

  player->cTransform->pos += player->cTransform->velocity;
  player->cTransform->angle += 2;
}
