#include "backends/graphics_opengl.h"

namespace ColorShaderSources
{
  const std::string vertex = R"---(

#version 330 core
layout (location = 0) in vec3 position;

uniform mat3 transform;

void
main()
{
  gl_Position = vec4(transform * vec3(position.xy, 1.0), 1.0);
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
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinates;

uniform mat3 transform;

out vec2 uv;

void
main()
{
  gl_Position = vec4(transform * vec3(position.xy, 1.0), 1.0);
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
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinates;

uniform mat3 transform;

out vec2 uv;

void
main()
{
  gl_Position = vec4(transform * vec3(position.xy, 1.0), 1.0);
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

namespace ModelShaderSources
{
  const std::string vertex = R"---(

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinates;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view_proj;

out vec2 uv;

void
main()
{
  gl_Position = view_proj * model * vec4(position, 1.0);
  uv = texture_coordinates;
}

  )---";

  const std::string fragment = R"---(

#version 330 core
layout(location = 0) out vec4 frag_color;

uniform vec3 color;
in vec2 uv;

void
main()
{
  frag_color = vec4(color, 1);
}

  )---";
}

GraphicsLayerOpenGL::TextureBinding::TextureBinding(Texture *_texture_data)
  : texture_data(_texture_data)
{
  unsigned int width = texture_data->get_width();
  unsigned int height = texture_data->get_height();
  unsigned int channels = texture_data->get_channels();
  const unsigned char *data = texture_data->get_data();

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  if (channels == 1)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, /**/ GL_RED /**/, width, height, 0,
      /**/ GL_RED /**/, GL_UNSIGNED_BYTE, data);
  }
  else if (channels == 3)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, /**/ GL_RGB /**/, width, height, 0,
      /**/ GL_RGB /**/, GL_UNSIGNED_BYTE, data);
  }
  else if (channels == 4)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, /**/ GL_RGBA /**/, width, height, 0,
      /**/ GL_RGBA /**/, GL_UNSIGNED_BYTE, data);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
}

GraphicsLayerOpenGL::TextureBinding::~TextureBinding()
{
  glDeleteTextures(1, &texture);
}

GraphicsLayerOpenGL::RenderTarget::RenderTarget()
{
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  glGenTextures(1, &target_texture);
  glBindTexture(GL_TEXTURE_2D, target_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target_texture, 0);
  GLenum draw_buffers[] = {
    GL_COLOR_ATTACHMENT0
  };
  glDrawBuffers(1, draw_buffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GraphicsLayerOpenGL::RenderTarget::~RenderTarget()
{
  glDeleteTextures(1, &target_texture);
  glDeleteRenderbuffers(1, &depth_buffer);
  glDeleteFramebuffers(1, &framebuffer);
}

void
GraphicsLayerOpenGL::RenderTarget::make_active()
{
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, 1280, 720);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
GraphicsLayerOpenGL::Shader::bind_uniform(float x, std::string name)
{
  glUseProgram(program);
  glUniform1fv(glGetUniformLocation(program, name.c_str()), 1, &x);
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Vec2 x, std::string name)
{
  glUseProgram(program);
  glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Vec3 x, std::string name)
{
  glUseProgram(program);
  glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Vec4 x, std::string name)
{
  glUseProgram(program);
  glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Mat3 x, std::string name)
{
  glUseProgram(program);
  glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()),
    1, GL_FALSE, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(Mat4 x, std::string name)
{
  glUseProgram(program);
  glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()),
    1, GL_FALSE, (float *)(&x));
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(const TextureBinding *x, std::string name)
{
  glUseProgram(program);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, x->texture);
  glUniform1i(glGetUniformLocation(program, name.c_str()), 0);
}

void
GraphicsLayerOpenGL::Shader::bind_uniform(const RenderTarget *x, std::string name)
{
  glUseProgram(program);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, x->target_texture);
  glUniform1i(glGetUniformLocation(program, name.c_str()), 0);
}

GraphicsLayerOpenGL::MeshBinding::MeshBinding(Mesh *_mesh)
{
  mesh = _mesh;

  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex),
    mesh->vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int),
    mesh->indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vec3));
  glEnableVertexAttribArray(1);
}

GraphicsLayerOpenGL::MeshBinding::~MeshBinding()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);

  glDeleteVertexArrays(1, &vao);
}

void
GraphicsLayerOpenGL::MeshBinding::draw(Shader *shader)
{
  shader->use();
  glBindVertexArray(vao);

  if (mesh->materials.size() == 0)
  {
    shader->bind_uniform(Vec3(1), "color");
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
  }
  else
  {
    glDepthFunc(GL_LESS);

    // Loop through materials, drawing each as a contiguous block of faces
    uint32_t offset = 0;
    for (uint32_t i = 0; i < mesh->materials.size(); ++i)
    {
      if (mesh->materials[i].vertices == 0)
        continue;
      shader->bind_uniform(mesh->materials[i].diffuse_color, "color");
      glDrawElements(GL_TRIANGLES, mesh->materials[i].vertices, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * offset));
      offset += mesh->materials[i].vertices;
    }
  }
}

GraphicsLayerOpenGL::GraphicsLayerOpenGL()
{
  target_3d = new RenderTarget();
  model_shader = new Shader(ModelShaderSources::vertex,
    ModelShaderSources::fragment);

  color_shader = new Shader(ColorShaderSources::vertex,
    ColorShaderSources::fragment);
  texture_shader = new Shader(TextureShaderSources::vertex,
    TextureShaderSources::fragment);
  text_shader = new Shader(TextShaderSources::vertex,
    TextShaderSources::fragment);
}

GraphicsLayerOpenGL::~GraphicsLayerOpenGL()
{
  delete target_3d;
  delete model_shader;

  delete color_shader;
  delete texture_shader;
  delete text_shader;
}

void
GraphicsLayerOpenGL::set_graphics_server(GraphicsServer *_graphics_server)
{
  graphics_server = _graphics_server;
}

void
GraphicsLayerOpenGL::bind_texture(Texture *tex)
{
  TextureBinding *binding = new TextureBinding(tex);
  set_texture_binding(tex, binding);
}

BoundMesh *
GraphicsLayerOpenGL::bind_mesh(Mesh *mesh)
{
  MeshBinding *binding = new MeshBinding(mesh);
  return binding;
}

Camera c;
BoundMesh *m;

#include "state.h"
#include "input.h"

void
GraphicsLayerOpenGL::begin_render()
{
  Vec2 viewport_size = graphics_server->get_framebuffer_size(false);
  glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO: proper api for managing 3d renders
  target_3d->make_active();
  glEnable(GL_DEPTH_TEST);

  {
    // TODO: don't temporarily render random cubes!
    if (m == nullptr)
    {
      c.position = Vec3(-8, 0, 0);
      c.direction = Vec3(1, 0, 0);
      c.clip_near = 0.1f;
      c.clip_far = 100.0f;

      ResourceBundle *globals = GameState::get()->get_globals();
      Scene *sc = (Scene *)globals->get_resource("test_scene");
      Mesh *d = sc->get_mesh();
      m = graphics_server->bind(d);
    }
    if (InputMonitor::get()->is_key_down(KeyW))
      c.position += Vec3(0.01, 0, 0);
    if (InputMonitor::get()->is_key_down(KeyS))
      c.position += Vec3(-0.01, 0, 0);
    if (InputMonitor::get()->is_key_down(KeyA))
      c.position += Vec3(0, 0, -0.01);
    if (InputMonitor::get()->is_key_down(KeyD))
      c.position += Vec3(0, 0, 0.01);
    if (InputMonitor::get()->is_key_down(KeySpace))
      c.position += Vec3(0, 0.01, 0);
    if (InputMonitor::get()->is_key_down(KeyN))
      c.position += Vec3(0, -0.01, 0);

    model_shader->bind_uniform(Mat4::identity(), "model");
    model_shader->bind_uniform(c.get_view_projection_matrix(), "view_proj");
    ((MeshBinding *)m)->draw(model_shader);
  }

  glDisable(GL_DEPTH_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 1280, 720);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  texture_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
    * Mat3::translate(Vec2(0, 0))
    * Mat3::scale(Vec2(1280, 720)), "transform");
  texture_shader->bind_uniform(target_3d, "sampler");
  ((MeshBinding *)graphics_server->get_quad())->draw(texture_shader);
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
  ((MeshBinding *)graphics_server->get_quad())->draw(color_shader);
}

void
GraphicsLayerOpenGL::draw_texture_rect(Vec2 origin, Vec2 size,
    const Texture &texture)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  texture_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
    * Mat3::translate(origin)
    * Mat3::scale(size), "transform");
  texture_shader->bind_uniform((TextureBinding *)get_texture_binding(&texture),
    "sampler");
  ((MeshBinding *)graphics_server->get_quad())->draw(texture_shader);
}

void
GraphicsLayerOpenGL::draw_character(Vec2 origin, Vec2 size, Vec4 color,
  const Texture &sdf)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  text_shader->bind_uniform(color, "color");
  text_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
    * Mat3::translate(origin)
    * Mat3::scale(size), "transform");
  text_shader->bind_uniform((TextureBinding *)get_texture_binding(&sdf), "sdf");
  ((MeshBinding *)graphics_server->get_quad())->draw(text_shader);
}

void
GraphicsLayerOpenGL::clear_mask()
{
  glClear(GL_STENCIL_BUFFER_BIT);
  glDisable(GL_STENCIL_TEST);
}

void
GraphicsLayerOpenGL::mask_rect(Vec2 origin, Vec2 size)
{
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  color_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
    * Mat3::translate(origin)
    * Mat3::scale(size), "transform");
  color_shader->bind_uniform(Vec4(0), "color");
  ((MeshBinding *)graphics_server->get_quad())->draw(color_shader);

  glStencilMask(0x00);
  glStencilFunc(GL_EQUAL, 1, 0xFF);
}
