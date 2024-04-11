#pragma once
#include "Vec2.h"
#include <iostream>
#include <valarray>

Vec2::Vec2() = default;

/**
 * \brief 
 * \param x Floating point x-coordinate of vector
 * \param y Floating point y-coordinate of vector
 */
Vec2::Vec2(float x, float y): x(x), y(y) {}

bool Vec2::operator==(const Vec2& rhs) const {
  return (x == rhs.x) && (y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const {
  return (x != rhs.x) || (y != rhs.y);
}

Vec2 Vec2::operator+(const float value) const {
  return {x + value, y + value};
}

Vec2 Vec2::operator-(const float value) const {
  return {x - value, y - value};
}

Vec2 Vec2::operator+(const Vec2& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

Vec2 Vec2::operator-(const Vec2& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

Vec2 Vec2::operator*(const float value) const {
  return {x * value, y * value};
}

Vec2 Vec2::operator/(const float value) const {
  return {x / value, y / value};
}

void Vec2::operator+=(const Vec2& rhs) {
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs) {
  x -= rhs.x;
  y -= rhs.y;
}

void Vec2::operator*=(const float value) {
  x = x * value;
  y = y * value;
}

void Vec2::operator/=(const float value) {
  x = x / value;
  y = y / value;
}

void Vec2::operator%=(const Vec2& rhs) {
  if (x > rhs.x) x = rhs.x;
  if (y > rhs.y) y = rhs.y;
}

void Vec2::operator%=(const float value) {
  if (x > value) x = value;
  if (y > value) y = value;
}

Vec2 Vec2::normalize() const {
  const float distance = std::sqrt(x * x + y * y);
  return {x / distance, y / distance};
}
