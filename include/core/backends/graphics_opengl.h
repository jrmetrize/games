#ifndef GRAPHICS_OPENGL_H
#define GRAPHICS_OPENGL_H

#include "core/graphics.h"
#include "core/linear_algebra.h"
#include <string>
#include <GLFW/glfw3.h>

class GraphicsLayerOpenGL : public GraphicsLayer
{
  GraphicsServer *graphics_server;

  struct TextureBinding
  {
    Texture *texture_data;
    GLuint texture;

    TextureBinding(Texture *_texture_data);

    ~TextureBinding();
  };

  struct GBuffer
  {
    GLuint framebuffer;
    GLuint position;
    GLuint normal;
    GLuint albedo;
    GLuint depth_buffer;

    GBuffer(int width, int height);

    ~GBuffer();

    void
    make_active();
  };

  struct RenderTarget
  {
    GLuint framebuffer;
    GLuint target_texture;
    GLuint depth_buffer;

    RenderTarget(int width, int height);

    ~RenderTarget();

    void
    make_active();
  };

  struct Shader
  {
    GLuint program;

    Shader(const std::string &vertex_shader_source,
      const std::string &fragment_shader_source);

    ~Shader();

    void
    use();

    void
    bind_uniform(float x, std::string name);

    void
    bind_uniform(Vec2 x, std::string name);

    void
    bind_uniform(Vec3 x, std::string name);

    void
    bind_uniform(Vec4 x, std::string name);

    void
    bind_uniform(Mat3 x, std::string name);

    void
    bind_uniform(Mat4 x, std::string name);

    // TODO: if a shader needs more than one texture at a time, allow binding
    // multiple texture units
    void
    bind_uniform(const TextureBinding *x, std::string name);

    void
    bind_uniform(const RenderTarget *x, std::string name);

    void
    bind_uniform(const GBuffer *x, std::string name);
  };

  struct MeshBinding : public BoundMesh
  {
    GLuint vbo;
    GLuint instance_vbo;
    GLuint ebo;

    GLuint vao;

    MeshBinding(Mesh *_mesh, uint32_t instances);

    ~MeshBinding();

    void
    draw(Shader *shader);
  };

  // Window
  GLFWwindow *window;

  // 3D
  GBuffer *gbuffer;
  RenderTarget *target_3d;
  Shader *model_shader;
  Shader *directional_light_shader;
  Shader *point_light_shader;
  Shader *spot_light_shader;
  Shader *ambient_light_shader;

  // 2D
  Shader *color_shader;
  Shader *texture_shader;
  Shader *text_shader;
public:
  GraphicsLayerOpenGL();

  ~GraphicsLayerOpenGL();

  GLFWwindow *
  get_window();

  void
  set_graphics_server(GraphicsServer *_graphics_server);

  void
  bind_texture(Texture *tex);

  BoundMesh *
  bind_mesh(Mesh *mesh, uint32_t instances = 1);

  void
  begin_render();

  void
  draw_color_rect(Vec2 origin, Vec2 size, Vec4 color);

  void
  draw_texture_rect(Vec2 origin, Vec2 size, const Texture &texture);

  void
  draw_character(Vec2 origin, Vec2 size, Vec4 color,
    const Texture &sdf);

  void
  clear_mask();

  void
  mask_rect(Vec2 origin, Vec2 size);

  void
  draw_3d(const Render3DRequest &scene_request);
};

#endif
