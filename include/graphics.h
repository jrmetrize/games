#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>

#include "linear_algebra.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

class Mesh;

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

class GraphicsServer
{
  GLFWwindow *window;

  Shader *color_shader;
  Shader *texture_shader;

  Mesh *quad;

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
  draw();
};

#endif
