#include "backends/graphics_opengl.h"

namespace ColorShaderSources
{
  const std::string vertex = R"---(

#version 330 core
layout (location = 0) in vec2 position;

uniform mat3 transform;

void
main()
{
  gl_Position = vec4(transform * vec3(position, 1.0), 1.0);
}

  )---";

  const std::string fragment = R"---(

#version 330 core
out vec4 frag_color;

uniform vec4 color;

void
main()
{
  frag_color = color;
}

  )---";
}

namespace TextureShaderSources
{
  const std::string vertex = R"---(

#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coordinates;

uniform mat3 transform;

out vec2 uv;

void
main()
{
  gl_Position = vec4(transform * vec3(position, 1.0), 1.0);
  uv = texture_coordinates;
}

  )---";

  const std::string fragment = R"---(

#version 330 core
out vec4 frag_color;

in vec2 uv;

uniform sampler2D sampler;

void
main()
{
  frag_color = texture(sampler, uv);
}

  )---";
}

namespace TextShaderSources
{
  const std::string vertex = R"---(

#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coordinates;

uniform mat3 transform;

out vec2 uv;

void
main()
{
  gl_Position = vec4(transform * vec3(position, 1.0), 1.0);
  uv = texture_coordinates;
}

  )---";

  const std::string fragment = R"---(

#version 330 core
out vec4 frag_color;

in vec2 uv;

uniform vec4 color;
uniform sampler2D sdf;

#define HALF_SMOOTHING (1.0 / 32.0)
#define LOWER_STEP (0.5 - (HALF_SMOOTHING))
#define UPPER_STEP (0.5 + (HALF_SMOOTHING))

void
main()
{
  float distance = texture(sdf, uv).r;
  float alpha = smoothstep(LOWER_STEP, UPPER_STEP, distance);
  frag_color = vec4(color.rgb, alpha);
}

  )---";
}

GraphicsLayerOpenGL::Shader::Shader(const std::string &vertex_shader_source,
  const std::string &fragment_shader_source)
{
  /* TODO: exceptions for shader compilation or link errors */

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  {
    const char *vertex_shader_source_c = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_source_c, nullptr);
  }
  glCompileShader(vertex_shader);

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  {
    const char *fragment_shader_source_c = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fragment_shader_source_c, nullptr);
  }
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

GraphicsLayerOpenGL::Shader::~Shader()
{
  glDeleteProgram(program);
}

void
GraphicsLayerOpenGL::Shader::use()
{
  glUseProgram(program);
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(float x, std::string name) const
{
  glUseProgram(program);
  glUniform1fv(glGetUniformLocation(program, name.c_str()), 1, &x);
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Vec2 x, std::string name) const
{
  glUseProgram(program);
  glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Vec3 x, std::string name) const
{
  glUseProgram(program);
  glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Vec4 x, std::string name) const
{
  glUseProgram(program);
  glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Mat3 x, std::string name) const
{
  glUseProgram(program);
  glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()),
    1, GL_FALSE, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Mat4 x, std::string name) const
{
  glUseProgram(program);
  glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()),
    1, GL_FALSE, (float *)(&x));
}

/*void
GraphicsLayerOpenGL::Shader::bind_uniform(const Texture &x, std::string name) const
{
  glUseProgram(program);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, x.texture);
  glUniform1i(glGetUniformLocation(program, name.c_str()), 0);
}*/

GraphicsLayerOpenGL::MeshBinding::MeshBinding(Mesh *_mesh)
  : mesh(_mesh)
{
  const VertexVector &vertices = mesh->get_vertices();
  const IndexVector &indices = mesh->get_indices();

  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
    vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vec2));
  glEnableVertexAttribArray(1);
}

GraphicsLayerOpenGL::MeshBinding::~MeshBinding()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);

  glDeleteVertexArrays(1, &vao);
}

void
GraphicsLayerOpenGL::MeshBinding::draw()
{
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, mesh->get_indices().size(), GL_UNSIGNED_INT, 0);
}

GraphicsLayerOpenGL::GraphicsLayerOpenGL()
{
  color_shader = new Shader(ColorShaderSources::vertex,
    ColorShaderSources::fragment);
  texture_shader = new Shader(TextureShaderSources::vertex,
    TextureShaderSources::fragment);
  text_shader = new Shader(TextShaderSources::vertex,
    TextShaderSources::fragment);

  quad = Mesh::primitive_quad();
  quad_binding = new MeshBinding(quad);
}

GraphicsLayerOpenGL::~GraphicsLayerOpenGL()
{
  delete color_shader;
  delete texture_shader;
  delete text_shader;

  delete quad_binding;
  delete quad;
}

void
GraphicsLayerOpenGL::set_graphics_server(GraphicsServer *_graphics_server)
{
  graphics_server = _graphics_server;
}

void
GraphicsLayerOpenGL::draw_color_rect(Vec2 origin, Vec2 size, Vec4 color)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  color_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
    * Mat3::translate(origin)
    * Mat3::scale(size), "transform");
  color_shader->bind_uniform(color, "color");
  color_shader->use();
  quad_binding->draw();
}
