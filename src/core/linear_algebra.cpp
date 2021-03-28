#include <cmath>

#include "core/linear_algebra.h"

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

Vec2::Vec2(float _x) :
  x(_x), y(_x)
{

}

float
Vec2::norm() const
{
  return sqrt((x * x) + (y * y));
}

Vec2
Vec2::normalized() const
{
  return (1 / norm()) * (*this);
}

Vec2
Vec2::perp() const
{
  return Vec2(-y, x);
}

bool
Vec2::inside_rect(const Vec2 &origin, const Vec2 &size) const
{
  return (origin.x <= x) && (x <= origin.x + size.x)
    && (origin.y <= y) && (y <= origin.y + size.y);
}

std::string
Vec2::string() const
{
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Vec2
Vec2::operator - () const
{
  return Vec2(-x, -y);
}

float &
Vec2::operator [] (const unsigned int &i)
{
  if (i == 0)
    return x;
  else
    return y;
}

const float &
Vec2::operator [] (const unsigned int &i) const
{
  if (i == 0)
    return x;
  else
    return y;
}

Vec2
Vec2::operator + (const Vec2 &b) const
{
  return Vec2(x + b.x, y + b.y);
}

Vec2 &
Vec2::operator += (const Vec2 &b)
{
  *this = *this + b;
  return *this;
}

Vec2
Vec2::operator - (const Vec2 &b) const
{
  return Vec2(x - b.x, y - b.y);
}

float
Vec2::operator * (const Vec2 &b) const
{
  return (x * b.x) + (y * b.y);
}

bool
Vec2::operator == (const Vec2 &b) const
{
  return (x == b.x) && (y == b.y);
}

Vec2
operator * (const float &a, const Vec2 &b)
{
  return Vec2(a * b.x, a * b.y);
}

Vec3::Vec3() :
  x(0),
  y(0),
  z(0)
{

}

Vec3::Vec3(float _x, float _y, float _z) :
  x(_x),
  y(_y),
  z(_z)
{

}

Vec3::Vec3(float _x) :
  x(_x), y(_x), z(_x)
{

}

float
Vec3::norm() const
{
  return sqrt((x * x) + (y * y) + (z * z));
}

Vec3
Vec3::normalized() const
{
  return (1 / norm()) * (*this);
}

std::string
Vec3::string() const
{
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ", "
    + std::to_string(z) + ")";
}

Vec3
Vec3::cross(const Vec3 &v) const
{
  Vec3 r = Vec3();
  r.x = (y * v.z) - (z * v.y);
  r.y = (z * v.x) - (x * v.z);
  r.z = (x * v.y) - (y * v.x);
  return r;
}

Vec3
Vec3::operator - () const
{
  return Vec3(-x, -y, -z);
}

float &
Vec3::operator [] (const unsigned int &i)
{
  if (i == 0)
    return x;
  else if (i == 1)
    return y;
  else
    return z;
}

const float &
Vec3::operator [] (const unsigned int &i) const
{
  if (i == 0)
    return x;
  else if (i == 1)
    return y;
  else
    return z;
}

Vec3
Vec3::operator + (const Vec3 &b) const
{
  return Vec3(x + b.x, y + b.y, z + b.z);
}

Vec3 &
Vec3::operator += (const Vec3 &b)
{
  *this = *this + b;
  return *this;
}

Vec3
Vec3::operator - (const Vec3 &b) const
{
  return Vec3(x - b.x, y - b.y, z - b.z);
}

float
Vec3::operator * (const Vec3 &b) const
{
  return (x * b.x) + (y * b.y) + (z * b.z);
}

bool
Vec3::operator == (const Vec3 &b) const
{
  return (x == b.x) && (y == b.y) && (z == b.y);
}

Vec3
operator * (const float &a, const Vec3 &b)
{
  return Vec3(a * b.x, a * b.y, a * b.z);
}

Vec4::Vec4() :
  x(0),
  y(0),
  z(0),
  w(0)
{

}

Vec4::Vec4(float _x, float _y, float _z, float _w) :
  x(_x),
  y(_y),
  z(_z),
  w(_w)
{

}

Vec4::Vec4(float _x) :
  x(_x), y(_x), z(_x), w(_x)
{

}

float
Vec4::norm() const
{
  return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

Vec3
Vec4::xyz() const
{
  return Vec3(x, y, z);
}

Vec4
Vec4::normalized() const
{
  return (1 / norm()) * (*this);
}

std::string
Vec4::string() const
{
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ", "
    + std::to_string(z) + ", " + std::to_string(w) + ")";
}

Vec4
Vec4::operator - () const
{
  return Vec4(-x, -y, -z, -w);
}

float &
Vec4::operator [] (const unsigned int &i)
{
  if (i == 0)
    return x;
  else if (i == 1)
    return y;
  else if (i == 2)
    return z;
  else
    return w;
}

const float &
Vec4::operator [] (const unsigned int &i) const
{
  if (i == 0)
    return x;
  else if (i == 1)
    return y;
  else if (i == 2)
    return z;
  else
    return w;
}

Vec4
Vec4::operator + (const Vec4 &b) const
{
  return Vec4(x + b.x, y + b.y, z + b.z, w + b.w);
}

Vec4 &
Vec4::operator += (const Vec4 &b)
{
  *this = *this + b;
  return *this;
}

Vec4
Vec4::operator - (const Vec4 &b) const
{
  return Vec4(x - b.x, y - b.y, z - b.z, w - b.w);
}

float
Vec4::operator * (const Vec4 &b) const
{
  return (x * b.x) + (y * b.y) + (z * b.z) + (w * b.w);
}

bool
Vec4::operator == (const Vec4 &b) const
{
  return (x == b.x) && (y == b.y) && (z == b.z) && (w == b.w);
}

Vec4
operator * (const float &a, const Vec4 &b)
{
  return Vec4(a * b.x, a * b.y, a * b.z, a * b.w);
}

Mat3::Mat3() :
  columns()
{

}

Vec3 &
Mat3::operator [] (const unsigned int &i)
{
  return columns[i];
}

const Vec3 &
Mat3::operator [] (const unsigned int &i) const
{
  return columns[i];
}

Mat3
Mat3::operator * (const Mat3 &b) const
{
  Mat3 m;
  for (unsigned int i = 0; i < 3; ++i)
  {
    for (unsigned int j = 0; j < 3; ++j)
    {
      Vec3 row;
      for (unsigned int k = 0; k < 3; ++k)
        row[k] = columns[k][i];
      Vec3 col = b[j];

      m[j][i] = row * col;
    }
  }
  return m;
}

Vec3
Mat3::operator * (const Vec3 &b) const
{
  Vec3 v;
  for (unsigned int i = 0; i < 3; ++i)
  {
    Vec3 row;
    for (unsigned int k = 0; k < 3; ++k)
      row[k] = columns[k][i];

    v[i] = row * b;
  }
  return v;
}

Mat3
Mat3::identity()
{
  Mat3 m = Mat3();
  m[0][0] = 1;
  m[1][1] = 1;
  m[2][2] = 1;
  return m;
}

Mat3
Mat3::translate(const Vec2 &v)
{
  Mat3 m = identity();
  m[2][0] = v.x;
  m[2][1] = v.y;
  return m;
}

Mat3
Mat3::scale(const Vec2 &v)
{
  Mat3 m = Mat3();
  m[0][0] = v.x;
  m[1][1] = v.y;
  m[2][2] = 1;
  return m;
}

Mat3
Mat3::rotate(const float &angle)
{
  Mat3 m = Mat3();
  m[0][0] = cos(angle);
  m[0][1] = sin(angle);
  m[1][0] = -sin(angle);
  m[1][1] = cos(angle);
  m[2][2] = 1;
  return m;
}

Mat4::Mat4() :
  columns()
{

}

Mat4
Mat4::identity()
{
  Mat4 m = Mat4();

  m.columns[0].x = 1;
  m.columns[1].y = 1;
  m.columns[2].z = 1;
  m.columns[3].w = 1;

  return m;
}


Mat4
Mat4::translation(Vec3 v)
{
  Mat4 m = Mat4();

  m.columns[0].x = 1;
  m.columns[1].y = 1;
  m.columns[2].z = 1;

  m.columns[3].x = v.x;
  m.columns[3].y = v.y;
  m.columns[3].z = v.z;
  m.columns[3].w = 1;

  return m;
}


Mat4
Mat4::scale(Vec3 v)
{
  Mat4 m = Mat4();

  m.columns[0].x = v.x;
  m.columns[1].y = v.y;
  m.columns[2].z = v.z;
  m.columns[3].w = 1;

  return m;
}


Mat4
Mat4::rotation(Vec3 axis, float angle)
{
  Mat4 m = Mat4();
  Vec3 u = axis.normalized();

  m[0] = Vec4(
    cos(angle) + ((u.x * u.x) * (1 - cos(angle))),
    ((u.y * u.x) * (1 - cos(angle))) + (u.z * sin(angle)),
    ((u.z * u.x) * (1 - cos(angle))) - (u.y * sin(angle)),
    0);
  m[1] = Vec4(
    ((u.x * u.y) * (1 - cos(angle))) - (u.z * sin(angle)),
    cos(angle) + ((u.y * u.y) * (1 - cos(angle))),
    ((u.z * u.y) * (1 - cos(angle))) + (u.x * sin(angle)),
    0);
  m[2] = Vec4(
    ((u.x * u.z) * (1 - cos(angle))) + (u.y * sin(angle)),
    ((u.y * u.z) * (1 - cos(angle))) - (u.x * sin(angle)),
    cos(angle) + ((u.z * u.z) * (1 - cos(angle))),
    0);
  m[3] = Vec4(0, 0, 0, 1);

  return m;
}


Mat4
Mat4::projection(float fov, float aspect, float near, float far)
{
  const float f = 1 / tan(fov / 2);
  Mat4 m = Mat4();

  m[0][0] = f / aspect;
  m[1][1] = -f;
  m[2][2] = (near + far) / (near - far);
  m[2][3] = -1;
  m[3][2] = (2 * near * far) / (near - far);

  return m;
}


Mat4
Mat4::lookat(Vec3 camera, Vec3 target, Vec3 up)
{
  Mat4 m = Mat4();

  Vec3 f = (target - camera).normalized();
  Vec3 u = up.normalized();
  Vec3 s = f.cross(u).normalized();
  u = s.normalized().cross(f);

  m[0] = Vec4(s.x, u.x, -f.x, 0);
  m[1] = Vec4(s.y, u.y, -f.y, 0);
  m[2] = Vec4(s.z, u.z, -f.z, 0);
  m[3] = Vec4(0, 0, 0, 1);

  Mat4 translate = Mat4::translation(-camera);

  return m * translate;
}

Vec4 &
Mat4::operator [] (const unsigned int &i)
{
  return columns[i];
}

const Vec4 &
Mat4::operator [] (const unsigned int &i) const
{
  return columns[i];
}

Mat4
Mat4::operator * (const Mat4 &b) const
{
  Mat4 m;
  for (unsigned int i = 0; i < 4; ++i)
  {
    for (unsigned int j = 0; j < 4; ++j)
    {
      Vec4 row;
      for (unsigned int k = 0; k < 4; ++k)
        row[k] = columns[k][i];
      Vec4 col = b[j];

      m[j][i] = row * col;
    }
  }
  return m;
}

Vec4
Mat4::operator * (const Vec4 &b) const
{
  Vec4 v;
  for (unsigned int i = 0; i < 4; ++i)
  {
    Vec4 row;
    for (unsigned int k = 0; k < 4; ++k)
      row[k] = columns[k][i];

    v[i] = row * b;
  }
  return v;
}
