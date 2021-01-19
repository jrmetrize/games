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
class FontFace;

class Texture
{
  friend class Shader;

  unsigned int texture;

  unsigned int width;
  unsigned int height;
  unsigned int channels;
public:
  Texture(std::string path);

  Texture(unsigned int _width, unsigned int _height, unsigned int _channels,
    const unsigned char *data);

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
  bind_uniform(const Texture &x, std::string name) const;
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
  Vec2 location;
};

struct RayInfo
{
  Vec2 origin;
  Vec2 direction; // must be normalized
};

class ComputedTexture
{
public:
  virtual Vec4
  value_at(Vec2 x) = 0;
};

class WhiteNoiseTexture : public ComputedTexture
{
  uint32_t seed;
public:
  WhiteNoiseTexture(uint32_t _seed);

  Vec4
  value_at(Vec2 x);
};

class Material
{
public:
  virtual Vec2
  normal_at(Vec2 x, Vec2 geometric_normal) = 0;
};

// TODO: make it possible to generalize this interface to shapes other than
// segments, like circles, isosurfaces, etc.
class RenderObject
{
public:
  virtual RayResult
  test_ray(RayInfo ray_info) const = 0;

  virtual Vec3
  get_color() const = 0;

  virtual Vec2
  get_normal() const = 0;
};

class Segment : public RenderObject
{
  Vec2 p1;
  Vec2 p2;
  Vec3 color;
public:
  Segment(Vec2 _p1, Vec2 _p2, Vec3 _color = Vec3(1, 1, 1));

  RayResult
  test_ray(RayInfo ray_info) const;

  Vec3
  get_color() const;

  void
  set_color(Vec3 _color);

  Vec2
  get_normal() const;
};

struct RenderTree
{
  std::vector<RenderObject *> objects;
  Vec2 sun_direction;
};

class Screen;

enum CameraMode
{
  Vertical = 0,
  Horizontal
};

struct RenderRequest
{
  CameraMode camera_mode;
  Vec2 camera_pos;
  Vec2 camera_dir;
  RenderTree tree;
};

struct TextRenderRequest
{
  Vec2 bounding_box_origin;
  Vec2 bounding_box_size;

  std::string text;
  float size;
  Vec4 color;
  FontFace *font;

  bool center;
};

class GraphicsServer
{
  GLFWwindow *window;

  Shader *color_shader;
  Shader *texture_shader;
  Shader *text_shader;

  Mesh *quad;

  Screen *current_screen;

  Vec4
  render_ray(RenderRequest to_render, RayInfo ray);
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

  void
  draw_color_rect(Vec2 origin, Vec2 size, Vec4 color);

  void
  draw_texture_rect(Vec2 origin, Vec2 size, const Texture &texture);

  void
  draw_text(const TextRenderRequest &text_request);

  void
  render_to_rect(Vec2 origin, Vec2 size, RenderRequest to_render);
};

#endif
