#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

struct Vec2
{
  float x;
  float y;

  Vec2();

  Vec2(float _x, float _y);

  Vec2
  operator - () const;

  Vec2
  operator + (const Vec2 &b) const;

  Vec2
  operator - (const Vec2 &b) const;
};

Vec2
operator * (const float &a, const Vec2 &b);

struct Vec3
{
  float x;
  float y;
  float z;
};

struct Vec4
{
  float x;
  float y;
  float z;
  float w;
};

#endif
