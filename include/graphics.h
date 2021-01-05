#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <functional>

#include "linear_algebra.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

class Mesh;
class Shader;

class Texture
{
  friend class Shader;

  unsigned int texture;

  unsigned int width;
  unsigned int height;
  unsigned int channels;
public:
  Texture(std::string path);

  ~Texture();
};

class Shader
{
  unsigned int program;
public:
  Shader(const std::string &vertex_shader_source,
    const std::string &fragment_shader_source);

  ~Shader();

  void
  draw(const Mesh *mesh) const;

  void
  bind_uniform(float x, std::string name) const;

  void
  bind_uniform(Vec2 x, std::string name) const;

  void
  bind_uniform(Vec3 x, std::string name) const;

  void
  bind_uniform(Vec4 x, std::string name) const;

  void
  bind_uniform(Mat3 x, std::string name) const;

  void
  bind_uniform(Mat4 x, std::string name) const;

  // TODO: if a shader needs more than one texture at a time, allow binding
  // multiple texture units
  void
  bind_uniform(Texture &x, std::string name) const;
};

struct Vertex
{
  Vec2 position;
  Vec2 texture_coordinates;

  Vertex(const Vec2 &_position, const Vec2 &_texture_coordinates);
};

using VertexVector = std::vector<Vertex>;
using IndexVector = std::vector<unsigned int>;

class Mesh
{
  VertexVector vertices;
  IndexVector indices;

  unsigned int vbo;
  unsigned int ebo;

  unsigned int vao;
public:
  Mesh(const VertexVector &_vertices, const IndexVector &_indices);

  ~Mesh();

  void
  draw() const;

  static Mesh *
  primitive_quad();
};

struct RayResult
{
  bool intersection;
  float distance;
};

struct RayInfo
{
  Vec2 origin;
  Vec2 direction; // must be normalized
};

class RenderObject
{
public:
  virtual RayResult
  test_ray(RayInfo ray_info) const = 0;
};

class Segment : public RenderObject
{
  Vec2 p1;
  Vec2 p2;
public:
  Segment(Vec2 _p1, Vec2 _p2);

  RayResult
  test_ray(RayInfo ray_info) const;
};

class RenderTree
{
  std::vector<RenderObject *> objects;
};

class Screen;

class GraphicsServer
{
  GLFWwindow *window;

  Shader *color_shader;
  Shader *texture_shader;

  Mesh *quad;

  Screen *current_screen;

  using RenderResult = std::vector<Vec3>;

  RenderResult
  render_world(Vec2 camera, Vec2 direction);
public:
  GraphicsServer(GLFWwindow *_window);

  ~GraphicsServer();

  Vec2
  get_framebuffer_size() const;

  Mat3
  get_pixel_to_screen_transform() const;

  Mat3
  get_screen_to_pixel_transform() const;

  void
  set_current_screen(Screen *screen);

  void
  draw();
};

#endif
