#include "graphics.h"
#include "glad/glad.h"

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

void
main()
{
  gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}

  )---";

  const std::string fragment = R"---(

#version 330 core
out vec4 frag_color;

void
main()
{
  frag_color = vec4(1.0, 0.0, 1.0, 1.0);
}

  )---";
}

Shader::Shader(const std::string &vertex_shader_source,
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

Shader::~Shader()
{
  glDeleteProgram(program);
}

void
Shader::draw(const Mesh *mesh) const
{
  glUseProgram(program);
  mesh->draw();
}

void
Shader::bind_uniform(float x, std::string name) const
{
  glUniform1fv(glGetUniformLocation(program, name.c_str()), 1, &x);
}

void
Shader::bind_uniform(Vec2 x, std::string name) const
{
  glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
Shader::bind_uniform(Vec3 x, std::string name) const
{
  glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
Shader::bind_uniform(Vec4 x, std::string name) const
{
  glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
Shader::bind_uniform(Mat3 x, std::string name) const
{
  glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()),
    1, GL_FALSE, (float *)(&x));
}

void
Shader::bind_uniform(Mat4 x, std::string name) const
{
  glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()),
    1, GL_FALSE, (float *)(&x));
}

Vertex::Vertex(const Vec2 &_position, const Vec2 &_texture_coordinates) :
  position(_position),
  texture_coordinates(_texture_coordinates)
{

}


Mesh::Mesh(const VertexVector &_vertices, const IndexVector &_indices) :
  vertices(_vertices),
  indices(_indices)
{
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

Mesh::~Mesh()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);

  glDeleteVertexArrays(1, &vao);
}

void
Mesh::draw() const
{
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh *
Mesh::primitive_quad()
{
  VertexVector vertices = {
    Vertex(Vec2(-1.0, -1.0), Vec2(0.0, 0.0)),
    Vertex(Vec2(1.0, -1.0), Vec2(1.0, 0.0)),
    Vertex(Vec2(-1.0, 1.0), Vec2(0.0, 1.0)),
    Vertex(Vec2(1.0, 1.0), Vec2(1.0, 1.0))
  };

  IndexVector indices = {
    0, 1, 2,
    1, 2, 3
  };

  return new Mesh(vertices, indices);
}

GraphicsServer::GraphicsServer()
{
  color_shader = new Shader(ColorShaderSources::vertex,
    ColorShaderSources::fragment);
  texture_shader = new Shader(TextureShaderSources::vertex,
    TextureShaderSources::fragment);
  quad = Mesh::primitive_quad();
}

GraphicsServer::~GraphicsServer()
{
  delete color_shader;
  delete texture_shader;
  delete quad;
}

void
GraphicsServer::draw()
{
  color_shader->bind_uniform(Mat3::rotate(3.14159 / 3.0) * Mat3::scale(Vec2(0.5, 0.5)), "transform");
  color_shader->bind_uniform(Vec4(1.0, 1.0, 0.0, 1.0), "color");
  color_shader->draw(quad);
  //texture_shader->draw(quad);
}
