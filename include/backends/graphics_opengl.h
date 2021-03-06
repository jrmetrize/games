#ifndef GRAPHICS_OPENGL_H
#define GRAPHICS_OPENGL_H

#include "graphics.h"
#include "linear_algebra.h"
#include <string>

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
  };

  Shader *color_shader;
  Shader *texture_shader;
  Shader *text_shader;

  struct MeshBinding
  {
    Mesh *mesh;

    GLuint vbo;
    GLuint ebo;

    GLuint vao;

    MeshBinding(Mesh *_mesh);

    ~MeshBinding();

    void
    draw();
  };

  Mesh *quad;
  MeshBinding *quad_binding;
public:
  GraphicsLayerOpenGL();

  ~GraphicsLayerOpenGL();

  void
  set_graphics_server(GraphicsServer *_graphics_server);

  void
  bind_texture(Texture *tex);

  void
  draw_color_rect(Vec2 origin, Vec2 size, Vec4 color);

  void
  draw_texture_rect(Vec2 origin, Vec2 size, const Texture &texture);

  void
  draw_character(Vec2 origin, Vec2 size, Vec4 color,
    const Texture &sdf);
};

#endif
