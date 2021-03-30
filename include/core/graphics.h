#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <functional>

#include "core/linear_algebra.h"
#include "FastNoiseLite.h"
#include "core/resource.h"

#include "core/glad/glad.h"

class GraphicsLayer;
class GraphicsServer;
class FontFace;
class Screen;
struct GLFWwindow;

class Texture
{
  friend class GraphicsLayer;

  unsigned int width;
  unsigned int height;
  unsigned int channels;
  const unsigned char *data;

  void *binding;
public:
  Texture(unsigned int _width, unsigned int _height, unsigned int _channels,
    const unsigned char *_data);

  ~Texture();

  unsigned int
  get_width() const;

  unsigned int
  get_height() const;

  unsigned int
  get_channels() const;

  const unsigned char *
  get_data() const;
};

struct BoundMesh
{
  const Mesh *mesh;

  virtual
  ~BoundMesh() = 0;
};

class DirectionalLight
{
public:
  // TODO: make accessors and stuff
  Vec3 position;

  Vec3 color;
  float intensity;
};

class SpotLight
{

};

class PointLight
{

};

struct Camera
{
  float fovy;
  float aspect_ratio;
  float clip_near;
  float clip_far;

  Vec3 position;
  Vec3 direction;

  Camera();

  Mat4
  get_view_projection_matrix();
};

class GraphicsLayer
{
protected:
  void
  set_texture_binding(Texture *tex, void *binding);

  void *
  get_texture_binding(const Texture *tex);
public:
  virtual
  ~GraphicsLayer() = 0;

  virtual GLFWwindow *
  get_window() = 0;

  virtual void
  set_graphics_server(GraphicsServer *_graphics_server) = 0;

  virtual void
  bind_texture(Texture *tex) = 0;

  virtual BoundMesh *
  bind_mesh(Mesh *mesh) = 0;

  virtual void
  begin_render() = 0;

  virtual void
  draw_color_rect(Vec2 origin, Vec2 size, Vec4 color) = 0;

  virtual void
  draw_texture_rect(Vec2 origin, Vec2 size, const Texture &texture) = 0;

  virtual void
  draw_character(Vec2 origin, Vec2 size, Vec4 color,
    const Texture &sdf) = 0;

  // TODO: When it's needed, make a more robust API for masking
  virtual void
  clear_mask() = 0;

  virtual void
  mask_rect(Vec2 origin, Vec2 size) = 0;
};

struct TextRenderRequest
{
  Vec2 bounding_box_origin;
  Vec2 bounding_box_size;

  std::string text;
  float size;
  Vec4 color;
  FontFace *font;

  // TODO: do we need more text alignment options besides 'top left' and 'top center'?
  bool center;
  bool center_vertical;
};

class GraphicsServer
{
  static GraphicsServer *instance;

  GraphicsLayer *backend;

  BoundMesh *quad;

  Screen *current_screen;
public:
  GraphicsServer();

  ~GraphicsServer();

  static void
  set_instance(GraphicsServer *_instance);

  static GraphicsServer *
  get();

  GLFWwindow *
  get_window();

  void
  bind(Texture *tex);

  BoundMesh *
  bind(Mesh *mesh);

  BoundMesh *
  get_quad();

  void
  set_fullscreen(bool fullscreen);

  Vec2
  get_scale() const;

  Vec2
  get_framebuffer_size(bool scaled = true) const;

  void
  window_resize(Vec2 size);

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
  draw_text_line(const TextRenderRequest &text_request);

  void
  draw_text(const TextRenderRequest &text_request);

  void
  clear_stencil_buffer();

  void
  draw_stencil_rect(Vec2 origin, Vec2 size);
};

#endif
