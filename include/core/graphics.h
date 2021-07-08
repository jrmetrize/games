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
public:
  enum Filtering
  {
    Nearest,
    Linear
  };
private:
  friend class GraphicsLayer;

  unsigned int width;
  unsigned int height;
  unsigned int channels;
  const unsigned char *data;
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

class BoundTexture
{
  const Texture *texture;
protected:
  Texture::Filtering filtering;
public:
  BoundTexture(const Texture *_texture);

  virtual
  ~BoundTexture() = 0;

  const Texture *
  get_texture() const;

  Texture::Filtering
  get_filtering();

  virtual void
  set_filtering(Texture::Filtering _filtering) = 0;
};

struct BoundMesh
{
  const Mesh *mesh;

  virtual
  ~BoundMesh() = 0;
};

/* Material parameters inspired by
   https://docs.godotengine.org/en/stable/classes/class_spatialmaterial.html#class-spatialmaterial */
class BoundMaterial
{
  const BoundTexture *albedo;
  const BoundTexture *metallic;
  const BoundTexture *normal;
  const BoundTexture *roughness;
public:

};

class Camera
{
  float fovy;
  float aspect_ratio;
  float clip_near;
  float clip_far;

  Vec3 position;
  Vec3 direction;
public:
  Camera();

  void
  set_fovy(float _fovy);

  void
  set_aspect_ratio(float _aspect_ratio);

  void
  set_clip_near(float _clip_near);

  void
  set_clip_far(float _clip_far);

  Vec3
  get_position() const;

  void
  set_position(Vec3 _position);

  void
  set_direction(Vec3 _direction);

  Mat4
  get_view_projection_matrix() const;
};

class SceneObject
{
public:
  SceneObject();

  BoundMesh *mesh;

  Mat4 transform;
};

class DirectionalLight
{
public:
  // TODO: make accessors and stuff
  Vec3 direction;

  Vec3 color;
  float intensity;
};

class SpotLight
{

};

class PointLight
{

};

class Scene3D
{
  Camera *camera;

  Vec3 ambient_color;

  std::vector<SceneObject *> objects;
  std::vector<DirectionalLight *> lights;
public:
  Scene3D(Camera *_camera);

  ~Scene3D();

  const Camera *
  get_camera() const;

  Vec3
  get_ambient_color() const;

  void
  set_ambient_color(Vec3 _ambient_color);

  std::vector<SceneObject *> &
  get_objects();

  const std::vector<SceneObject *> &
  get_objects() const;

  std::vector<DirectionalLight *> &
  get_lights();

  const std::vector<DirectionalLight *> &
  get_lights() const;
};

struct Render3DRequest
{
  Vec2 quad_origin;
  Vec2 quad_size;

  Scene3D *scene;
};

class GraphicsLayer
{
public:
  virtual
  ~GraphicsLayer() = 0;

  virtual GLFWwindow *
  get_window() = 0;

  virtual void
  window_resize(Vec2 size) = 0;

  virtual void
  set_graphics_server(GraphicsServer *_graphics_server) = 0;

  virtual BoundTexture *
  bind_texture(Texture *tex) = 0;

  virtual BoundMesh *
  bind_mesh(Mesh *mesh, uint32_t instances = 1) = 0;

  virtual void
  begin_render() = 0;

  virtual void
  draw_color_rect(Vec2 origin, Vec2 size, Vec4 color) = 0;

  virtual void
  draw_texture_rect(Vec2 origin, Vec2 size, const BoundTexture &texture) = 0;

  virtual void
  draw_character(Vec2 origin, Vec2 size, Vec4 color,
    const BoundTexture &sdf) = 0;

  // TODO: When it's needed, make a more robust API for masking
  virtual void
  clear_mask() = 0;

  virtual void
  mask_rect(Vec2 origin, Vec2 size) = 0;

  virtual void
  draw_3d(const Render3DRequest &scene_request) = 0;
};

class BoundFont;

struct TextRenderRequest
{
  Vec2 bounding_box_origin;
  Vec2 bounding_box_size;

  std::string text;
  float size;
  Vec4 color;
  BoundFont *font;

  // TODO: do we need more text alignment options besides 'top left' and 'top center'?
  bool center;
  bool center_vertical;

  bool cursor;
  uint32_t cursor_pos;
  Vec4 cursor_color;

  /* For drawing text input boxes */
  Vec2 scroll_offset;
  bool mask_bounds;
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

  BoundTexture *
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
  draw_texture_rect(Vec2 origin, Vec2 size, const BoundTexture &texture);

  void
  draw_text_line(const TextRenderRequest &text_request);

  void
  draw_text(const TextRenderRequest &text_request);

  void
  clear_stencil_buffer();

  void
  draw_stencil_rect(Vec2 origin, Vec2 size);

  void
  draw_3d(const Render3DRequest &scene_request);
};

#endif
