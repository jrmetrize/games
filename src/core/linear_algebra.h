#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <string>

struct Vec2
{
  float x;
  float y;

  Vec2();

  Vec2(float _x, float _y);

  Vec2(float _x);

  float
  norm() const;

  Vec2
  normalized() const;

  Vec2
  perp() const;

  bool
  inside_rect(const Vec2 &origin, const Vec2 &size) const;

  std::string
  string() const;

  Vec2
  operator - () const;

  float &
  operator [] (const unsigned int &i);

  const float &
  operator [] (const unsigned int &i) const;

  Vec2
  operator + (const Vec2 &b) const;

  Vec2 &
  operator += (const Vec2 &b);

  Vec2
  operator - (const Vec2 &b) const;

  float
  operator * (const Vec2 &b) const;

  bool
  operator == (const Vec2 &b) const;
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

  Vec3(float _x);

  float
  norm() const;

  Vec3
  normalized() const;

  std::string
  string() const;

  Vec3
  cross(const Vec3 &v) const;

  Vec3
  operator - () const;

  float &
  operator [] (const unsigned int &i);

  const float &
  operator [] (const unsigned int &i) const;

  Vec3
  operator + (const Vec3 &b) const;

  Vec3 &
  operator += (const Vec3 &b);

  Vec3
  operator - (const Vec3 &b) const;

  float
  operator * (const Vec3 &b) const;

  bool
  operator == (const Vec3 &b) const;
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

  Vec4(float _x);

  float
  norm() const;

  Vec3
  xyz() const;

  Vec4
  normalized() const;

  std::string
  string() const;

  Vec4
  operator - () const;

  float &
  operator [] (const unsigned int &i);

  const float &
  operator [] (const unsigned int &i) const;

  Vec4
  operator + (const Vec4 &b) const;

  Vec4 &
  operator += (const Vec4 &b);

  Vec4
  operator - (const Vec4 &b) const;

  float
  operator * (const Vec4 &b) const;

  bool
  operator == (const Vec4 &b) const;
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

  static Mat4
  identity();

  static Mat4
  translation(Vec3 v);

  static Mat4
  scale(Vec3 v);

  static Mat4
  rotation(Vec3 axis, float angle);

  static Mat4
  projection(float fov, float aspect, float near, float far);

  static Mat4
  lookat(Vec3 camera, Vec3 target, Vec3 up);

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
