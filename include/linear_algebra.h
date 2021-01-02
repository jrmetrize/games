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

  float &
  operator [] (const unsigned int &i);

  const float &
  operator [] (const unsigned int &i) const;

  Vec2
  operator + (const Vec2 &b) const;

  Vec2
  operator - (const Vec2 &b) const;

  float
  operator * (const Vec2 &b) const;
};

Vec2
operator * (const float &a, const Vec2 &b);

struct Vec3
{
  float x;
  float y;
  float z;

  Vec3();

  Vec3(float _x, float _y, float _z);

  Vec3
  operator - () const;

  float &
  operator [] (const unsigned int &i);

  const float &
  operator [] (const unsigned int &i) const;

  Vec3
  operator + (const Vec3 &b) const;

  Vec3
  operator - (const Vec3 &b) const;

  float
  operator * (const Vec3 &b) const;
};

Vec3
operator * (const float &a, const Vec3 &b);

struct Vec4
{
  float x;
  float y;
  float z;
  float w;

  Vec4();

  Vec4(float _x, float _y, float _z, float _w);

  Vec4
  operator - () const;

  float &
  operator [] (const unsigned int &i);

  const float &
  operator [] (const unsigned int &i) const;

  Vec4
  operator + (const Vec4 &b) const;

  Vec4
  operator - (const Vec4 &b) const;

  float
  operator * (const Vec4 &b) const;
};

Vec4
operator * (const float &a, const Vec4 &b);

struct Mat3
{
  Vec3 columns[3];

  Mat3();

  Vec3 &
  operator [] (const unsigned int &i);

  const Vec3 &
  operator [] (const unsigned int &i) const;

  Mat3
  operator * (const Mat3 &b) const;

  Vec3
  operator * (const Vec3 &b) const;

  static Mat3
  identity();

  static Mat3
  translate(const Vec2 &v);

  static Mat3
  scale(const Vec2 &v);

  static Mat3
  rotate(const float &angle);
};

struct Mat4
{
  Vec4 columns[4];

  Mat4();

  Vec4 &
  operator [] (const unsigned int &i);

  const Vec4 &
  operator [] (const unsigned int &i) const;

  Mat4
  operator * (const Mat4 &b) const;

  Vec4
  operator * (const Vec4 &b) const;
};

#endif
