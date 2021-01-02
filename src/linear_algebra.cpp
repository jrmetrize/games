#include "linear_algebra.h"

Vec2::Vec2() :
  x(0),
  y(0)
{

}

Vec2::Vec2(float _x, float _y) :
  x(_x),
  y(_y)
{

}

Vec2
Vec2::operator - () const
{
  return Vec2(-x, -y);
}

Vec2
Vec2::operator + (const Vec2 &b) const
{
  return Vec2(x + b.x, y + b.y);
}

Vec2
Vec2::operator - (const Vec2 &b) const
{
  return Vec2(x - b.x, y - b.y);
}

Vec2
operator * (const float &a, const Vec2 &b)
{
  return Vec2(a * b.x, a * b.y);
}
