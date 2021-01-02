#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>

#include "linear_algebra.h"

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
  Shader *texture_shader;

  Mesh *quad;
public:
  GraphicsServer();

  ~GraphicsServer();

  void
  draw();
};

#endif
