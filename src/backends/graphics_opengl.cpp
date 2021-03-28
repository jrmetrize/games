#include "backends/graphics_opengl.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
drop a is
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

out vec3 world_pos;
out vec3 world_normal;
out vec2 uv;

void
main()
{
  gl_Position = view_proj * model * vec4(position, 1.0);
  world_pos = (model * vec4(position, 1.0)).xyz;
  world_normal = normal;
  uv = texture_coordinates;
}

  )---";

  const std::string fragment = R"---(

#version 330 core
layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 albedo;

uniform vec3 color;

in vec3 world_pos;
in vec3 world_normal;
in vec2 uv;

void
main()
{
  position = vec4(world_pos, 1.0);
  normal = vec4(world_normal, 1.0);
  albedo = vec4(0.3, 0.4, 0.25, 1.0);
}

  )---";
}

namespace LightingShaderSources
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

  const std::string dir_fragment = R"---(

#version 330 core
out vec4 frag_color;

in vec2 uv;

uniform sampler2D position;
uniform sampler2D normal_tex;
uniform sampler2D albedo_tex;

uniform vec3 light_dir;
uniform vec3 light_color;

uniform vec3 camera_pos;

void
main()
{
  vec3 pixel_pos = texture(position, uv).xyz;
  vec3 normal = texture(normal_tex, uv).xyz;
  vec3 albedo = texture(albedo_tex, uv).xyz;

  vec3 camera_dir = normalize(camera_pos - pixel_pos);
  vec3 halfway = normalize(light_dir + camera_dir);

  float diffuse = max(dot(light_dir, normal), 0);
  vec3 diffuse_color = light_color * diffuse;

  float specular = pow(max(dot(halfway, normal), 0), 16);
  vec3 specular_color = light_color * specular;

  frag_color = vec4((diffuse_color + specular_color) * albedo, 1);
}

  )---";

  const std::string point_fragment = R"---(

#version 330 core
out vec4 frag_color;

in vec2 uv;

uniform sampler2D position;
uniform sampler2D normal_tex;
uniform sampler2D albedo_tex;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 camera_pos;

void
main()
{
  vec3 pixel_pos = texture(position, uv).xyz;
  vec3 normal = texture(normal_tex, uv).xyz;
  vec3 albedo = texture(albedo_tex, uv).xyz;

  vec3 light_dir = normalize(light_pos - pixel_pos);
  vec3 camera_dir = normalize(camera_pos - pixel_pos);
  vec3 halfway = normalize(light_dir + camera_dir);

  float diffuse = max(dot(light_dir, normal), 0);
  vec3 diffuse_color = light_color * diffuse;

  float specular = pow(max(dot(halfway, normal), 0), 16);
  vec3 specular_color = light_color * specular;

  frag_color = vec4((diffuse_color + specular_color) * albedo, 1);
}

  )---";

  const std::string spot_fragment = R"---(

#version 330 core
out vec4 frag_color;

in vec2 uv;

uniform sampler2D position;
uniform sampler2D normal_tex;
uniform sampler2D albedo_tex;

uniform vec3 light_pos;
uniform vec3 light_dir;
uniform float light_angle; // the cosine of the angle of the light cone.
uniform vec3 light_color;

uniform vec3 camera_pos;

void
main()
{
  vec3 pixel_pos = texture(position, uv).xyz;
  vec3 normal = texture(normal_tex, uv).xyz;
  vec3 albedo = texture(albedo_tex, uv).xyz;

  vec3 light_to_pixel_dir = normalize(light_pos - pixel_pos);
  light_to_pixel_dir = normalize(vec3(0, 0, 0) - pixel_pos);
  vec3 camera_dir = normalize(camera_pos - pixel_pos);
  vec3 halfway = normalize(light_to_pixel_dir + camera_dir);

  float visibility = dot(-light_dir, light_to_pixel_dir);
  /*if (dot(-light_dir, light_to_pixel_dir) >= 0.95)
    visibility = 1.0;*/

  /*float diffuse = max(dot(light_to_pixel_dir, normal), 0);
  vec3 diffuse_color = light_color * diffuse;

  float specular = pow(max(dot(halfway, normal), 0), 16);
  vec3 specular_color = light_color * specular;

  frag_color = vec4((diffuse_color + specular_color) * albedo * visibility, 1);*/
  frag_color = vec4(-light_to_pixel_dir.x * albedo, 1);
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

GraphicsLayerOpenGL::GBuffer::GBuffer(int width, int height)
{
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  glGenTextures(1, &position);
  glBindTexture(GL_TEXTURE_2D, position);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, position, 0);

  glGenTextures(1, &normal);
  glBindTexture(GL_TEXTURE_2D, normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, normal, 0);

  glGenTextures(1, &albedo);
  glBindTexture(GL_TEXTURE_2D, albedo);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, albedo, 0);

  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

  GLenum draw_buffers[] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2
  };
  glDrawBuffers(3, draw_buffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GraphicsLayerOpenGL::GBuffer::~GBuffer()
{
  glDeleteTextures(1, &position);
  glDeleteTextures(1, &normal);
  glDeleteTextures(1, &albedo);
  glDeleteRenderbuffers(1, &depth_buffer);
  glDeleteFramebuffers(1, &framebuffer);
}

void
GraphicsLayerOpenGL::GBuffer::make_active()
{
  Vec2 viewport_size = GraphicsServer::get()->get_framebuffer_size(false);

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GraphicsLayerOpenGL::RenderTarget::RenderTarget(int width, int height)
{
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  glGenTextures(1, &target_texture);
  glBindTexture(GL_TEXTURE_2D, target_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
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
  Vec2 viewport_size = GraphicsServer::get()->get_framebuffer_size(false);

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
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

void
GraphicsLayerOpenGL::Shader::bind_uniform(const GBuffer *x, std::string name)
{
  glUseProgram(program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, x->position);
  glUniform1i(glGetUniformLocation(program, "position"), 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, x->normal);
  glUniform1i(glGetUniformLocation(program, "normal_tex"), 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, x->albedo);
  glUniform1i(glGetUniformLocation(program, "albedo_tex"), 2);
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

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(2);
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
#include <iostream>
GraphicsLayerOpenGL::GraphicsLayerOpenGL()
{
  if (glfwInit() != GLFW_TRUE)
  {
    //error_dialog("Couldn't initialize GLFW.");
    //return 0;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(1280, 720, "jrCollection", nullptr, nullptr);
  if (window == nullptr)
  {
    //error_dialog("Couldn't create window.");
    glfwTerminate();
    //return 0;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    //error_dialog("Couldn't initialize OpenGL.");
    glfwTerminate();
    //return 0;
  }

  int scaled_width;
  int scaled_height;
  glfwGetFramebufferSize(window, &scaled_width, &scaled_height);

  // TODO: only use imgui in dev builds
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  gbuffer = new GBuffer(scaled_width, scaled_height);
  target_3d = new RenderTarget(scaled_width, scaled_height);
  model_shader = new Shader(ModelShaderSources::vertex,
    ModelShaderSources::fragment);
  directional_light_shader = new Shader(LightingShaderSources::vertex,
    LightingShaderSources::dir_fragment);
  point_light_shader = new Shader(LightingShaderSources::vertex,
    LightingShaderSources::point_fragment);
  spot_light_shader = new Shader(LightingShaderSources::vertex,
    LightingShaderSources::spot_fragment);

  color_shader = new Shader(ColorShaderSources::vertex,
    ColorShaderSources::fragment);
  texture_shader = new Shader(TextureShaderSources::vertex,
    TextureShaderSources::fragment);
  text_shader = new Shader(TextShaderSources::vertex,
    TextShaderSources::fragment);
}

GraphicsLayerOpenGL::~GraphicsLayerOpenGL()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  delete gbuffer;
  delete target_3d;
  delete model_shader;
  delete directional_light_shader;
  delete point_light_shader;
  delete spot_light_shader;

  delete color_shader;
  delete texture_shader;
  delete text_shader;

  glfwTerminate();
}

GLFWwindow *
GraphicsLayerOpenGL::get_window()
{
  return window;
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
#include <iostream>

void
GraphicsLayerOpenGL::begin_render()
{
  Vec2 viewport_size = graphics_server->get_framebuffer_size(false);
  Vec2 viewport_size_scaled = graphics_server->get_framebuffer_size();

  // TODO: proper api for managing 3d renders
  gbuffer->make_active();
  glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

  /* Now that the gbuffer is full, do the lighting pass */
  {
    // TODO: proper api for managing 3d renders
    target_3d->make_active();
    glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    point_light_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
      * Mat3::translate(Vec2(0, 0))
      * Mat3::scale(viewport_size_scaled), "transform");
    point_light_shader->bind_uniform(gbuffer, "x");
    point_light_shader->bind_uniform(Vec3(0, 5, 0), "light_pos");
    point_light_shader->bind_uniform(Vec3(1, 1, 1), "light_color");
    point_light_shader->bind_uniform(c.position, "camera_pos");
    ((MeshBinding *)graphics_server->get_quad())->draw(point_light_shader);

    directional_light_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
      * Mat3::translate(Vec2(0, 0))
      * Mat3::scale(viewport_size_scaled), "transform");
    directional_light_shader->bind_uniform(gbuffer, "x");
    directional_light_shader->bind_uniform(Vec3(3, -5, 0).normalized(), "light_dir");
    directional_light_shader->bind_uniform(Vec3(1, 0, 0), "light_color");
    directional_light_shader->bind_uniform(c.position, "camera_pos");
    ((MeshBinding *)graphics_server->get_quad())->draw(directional_light_shader);

    /*
    spot_light_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
      * Mat3::translate(Vec2(0, 0))
      * Mat3::scale(Vec2(1280, 720)), "transform");
    spot_light_shader->bind_uniform(gbuffer, "x");
    spot_light_shader->bind_uniform(Vec3(-5, 0, 0), "light_pos");
    spot_light_shader->bind_uniform(Vec3(1, 0, 0).normalized(), "light_dir");
    spot_light_shader->bind_uniform(0.98, "light_angle");
    spot_light_shader->bind_uniform(Vec3(0, 0, 1), "light_color");
    spot_light_shader->bind_uniform(c.position, "camera_pos");
    ((MeshBinding *)graphics_server->get_quad())->draw(spot_light_shader);
    */

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  texture_shader->bind_uniform(graphics_server->get_pixel_to_screen_transform()
    * Mat3::translate(Vec2(0, 0))
    * Mat3::scale(viewport_size_scaled), "transform");
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
