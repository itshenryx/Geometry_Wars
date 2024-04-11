#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform {
public:
  Vec2 pos = {0, 0};
  Vec2 velocity = {0, 0};
  float angle = 0;

  CTransform(const Vec2& p, const Vec2& velocity, float angle):
    pos(p), velocity(velocity), angle(angle) {}
};

class CShape {
public:
  sf::CircleShape shape;
  sf::Color color;
  int sides;

  CShape(float radius, int points, int r, int g, int b) :
    shape(radius, points), sides(points),
    color(r, g, b, 255) {
    shape.setFillColor(color);
    shape.setOrigin(radius, radius);
  }

  CShape(float radius, int points, sf::Color colors) :
    shape(radius, points), color(colors),
    sides(points) {
    shape.setFillColor(color);
    shape.setOrigin(radius, radius);
  }
};

class CLazer {
public:
  sf::RectangleShape shape;
  sf::Color color;
  float angle;
  float m, c;
  bool px, py;

  CLazer(float width) :
    shape(sf::Vector2f(width, 3)),
    color(255, 0, 0, 255) {
    shape.setFillColor(color);
    shape.setOrigin(0, 1);
  }
};


class CCollision {
public:
  float radius = 0;
  CCollision(float r) : radius(r) {}
};

class CInput {
public:
  bool up = 0, down = 0, left = 0, right = 0, shoot = 0, special = 0;
  CInput() = default;
};

class CScore {
public:
  int score = 0;
  CScore(int s): score(s) {}
};

class CLifespan {
public:
  float remaining = 0,
        total = 0;
  CLifespan(float total) : remaining(total), total(total) {}
};
