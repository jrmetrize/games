#ifndef GRAPHICS_OPENGL_H
#define GRAPHICS_OPENGL_H

#include "graphics.h"
#include "linear_algebra.h"
#include <string>

class GraphicsLayerOpenGL : public GraphicsLayer
{
  struct Shader
  {
    GLuint program;

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
    /*void
    bind_uniform(const Texture &x, std::string name) const;*/
  };

  Shader *color_shader;
  Shader *texture_shader;
  Shader *text_shader;

  struct MeshBinding
  {
    Mesh *mesh;

    unsigned int vbo;
    unsigned int ebo;

    unsigned int vao;

    /*Mesh(const VertexVector &_vertices, const IndexVector &_indices);

    ~Mesh();

    void
    draw() const;

    static Mesh *
    primitive_quad();*/
  };
public:
  GraphicsLayerOpenGL();

  ~GraphicsLayerOpenGL();
};

#endif
