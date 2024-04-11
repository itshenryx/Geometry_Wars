#pragma once

class Vec2 {
public:
  float x = 0,
        y = 0;

  Vec2();
  Vec2(float x, float y);

  bool operator ==(const Vec2& rhs) const;
  bool operator !=(const Vec2& rhs) const;

  Vec2 operator +(float value) const;
  Vec2 operator -(float value) const;

  Vec2 operator +(const Vec2& rhs) const;
  Vec2 operator -(const Vec2& rhs) const;
  Vec2 operator /(const float value) const;
  Vec2 operator *(const float value) const;

  void operator +=(const Vec2& rhs);
  void operator -=(const Vec2& rhs);
  void operator *=(const float value);
  void operator /=(const float value);

  void operator %=(const Vec2& rhs);
  void operator %=(float value);
  
  Vec2 normalize() const;
};
