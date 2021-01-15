#include "graphics.h"
#include "screen.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

#define HALF_SMOOTHING (1.0 / 16.0)
#define LOWER_STEP (0.5 - (HALF_SMOOTHING))
#define UPPER_STEP (0.5 + (HALF_SMOOTHING))

void
main()
{
  float distance = texture(sampler, uv).r;
  float alpha = smoothstep(LOWER_STEP, UPPER_STEP, distance);
  frag_color = vec4(vec3(1), alpha);
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
  gl_Position = vec4(transform * vec3(position, 1.0), 0.0, 1.0);
  uv = texture_coordinates;
}

  )---";

  const std::string fragment = R"---(

#version 330 core
out vec4 frag_color;

in vec2 uv;

uniform vec4 color;
uniform sampler2D sampler;

void
main()
{
  frag_color = vec4(color.rgb, texture(sampler, uv).r);
}

  )---";
}

Texture::Texture(std::string path)
{
  int _width;
  int _height;
  int _channels;

  unsigned char *img_data = stbi_load(path.c_str(),
    &_width, &_height, &_channels, 0);

  // TODO: check that img_data is not nullptr

  width = (unsigned int)_width;
  height = (unsigned int)_height;
  channels = (unsigned int)_channels;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, /**/ GL_RGB /**/, width, height, 0,
    /**/ GL_RGB /**/, GL_UNSIGNED_BYTE, img_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(img_data);
}

Texture::Texture(unsigned int _width, unsigned int _height, unsigned int _channels,
  const unsigned char *data) :
  width(_width), height(_height), channels(_channels)
{
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
  glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
  glDeleteTextures(1, &texture);
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

void
Shader::bind_uniform(const Texture &x, std::string name) const
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, x.texture);
  glUniform1i(glGetUniformLocation(program, name.c_str()), 0);
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
    Vertex(Vec2(0.0, 0.0), Vec2(0.0, 1.0)),
    Vertex(Vec2(1.0, 0.0), Vec2(1.0, 1.0)),
    Vertex(Vec2(0.0, 1.0), Vec2(0.0, 0.0)),
    Vertex(Vec2(1.0, 1.0), Vec2(1.0, 0.0))
  };

  IndexVector indices = {
    0, 1, 2,
    1, 2, 3
  };

  return new Mesh(vertices, indices);
}

Segment::Segment(Vec2 _p1, Vec2 _p2, Vec3 _color) :
  p1(_p1),
  p2(_p2),
  color(_color)
{

}

Vec3
Segment::get_color() const
{
  return color;
}

void
Segment::set_color(Vec3 _color)
{
  color = _color;
}

Vec2
Segment::get_normal() const
{
  Vec2 dir = p2 - p1;
  Vec2 normal = Vec2(-dir.x, dir.y);
  return normal.normalized();
}

RayResult
Segment::test_ray(RayInfo ray_info) const
{
  RayResult result = {};

  Vec2 d = ray_info.direction;
  Vec2 o = ray_info.origin;
  Vec2 v = p2 - p1;

  float q = v * Vec2(d.y, -d.x);

  float t1 = (v * Vec2(p1.y - o.y, o.x - p1.x)) / q;
  float t2 = (d * Vec2(p1.y - o.y, o.x - p1.x)) / q;

  if (t1 >= 0 && (t2 >= 0 && t2 <= 1))
  {
    result.intersection = true;
    result.distance = t1;
    result.location = ray_info.origin + (t1 * ray_info.direction);
  }

  return result;
}

GraphicsServer::GraphicsServer(GLFWwindow *_window) :
  window(_window),
  current_screen(nullptr)
{
  color_shader = new Shader(ColorShaderSources::vertex,
    ColorShaderSources::fragment);
  texture_shader = new Shader(TextureShaderSources::vertex,
    TextureShaderSources::fragment);
  quad = Mesh::primitive_quad();

  // TODO: only use imgui in dev builds
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

GraphicsServer::~GraphicsServer()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  delete color_shader;
  delete texture_shader;
  delete quad;
}

Vec2
GraphicsServer::get_framebuffer_size() const
{
  int width;
  int height;
  glfwGetFramebufferSize(window, &width, &height);

  return Vec2(float(width), float(height));
}

Mat3
GraphicsServer::get_pixel_to_screen_transform() const
{
  Vec2 viewport_size = get_framebuffer_size();
  return Mat3::translate(Vec2(-1.0f, -1.0f)) *
    Mat3::scale(Vec2(2.0f / viewport_size.x, 2.0f / viewport_size.y));
}

Mat3
GraphicsServer::get_screen_to_pixel_transform() const
{
  Vec2 viewport_size = get_framebuffer_size();
  return Mat3::scale(Vec2(viewport_size.x / 2.0f, viewport_size.y / 2.0f)) *
    Mat3::translate(Vec2(1.0f, 1.0f));
}

void
GraphicsServer::set_current_screen(Screen *screen)
{
  current_screen = screen;
}

Vec4
GraphicsServer::render_ray(RenderRequest to_render, RayInfo ray)
{
  RayResult nearest;
  RenderObject *hit = nullptr;
  Vec4 color = Vec4(0, 0, 0, 1);
  for (unsigned int i = 0; i < to_render.tree.objects.size(); ++i)
  {
    RayResult r = to_render.tree.objects[i]->test_ray(ray);
    if (r.intersection)
    {
      if (nearest.distance < 0 || r.distance < nearest.distance)
      {
        nearest = r;
        hit = to_render.tree.objects[i];
      }
    }
  }
  if (hit != nullptr)
  {
    Vec3 obj_color = hit->get_color();
    color += Vec4(0.2 * obj_color.x, 0.2 * obj_color.y, 0.2 * obj_color.z, 0);
    // Now do lighting calculations!
    if (to_render.tree.sun_direction != Vec2())
    {
      RayInfo sun_ray = {};
      sun_ray.origin = nearest.location;
      sun_ray.direction = -to_render.tree.sun_direction;
      bool shadowed = false;
      for (unsigned int i = 0; i < to_render.tree.objects.size(); ++i)
      {
        if (to_render.tree.objects[i] == hit)
          continue;
        RayResult r = to_render.tree.objects[i]->test_ray(sun_ray);
        if (r.intersection)
        {
          shadowed = true;
          break;
        }
      }
      if (!shadowed)
      {
        // Calculate diffuse (using Blinn-Phong for now)
        // TODO: PBR for 2D?
        Vec2 normal = hit->get_normal();
        Vec2 light_dir = (-to_render.tree.sun_direction).normalized();
        float diffuse = std::max(normal * light_dir, 0.0f);
        color += Vec4(diffuse * obj_color.x, diffuse * obj_color.y,
          diffuse * obj_color.y, 0);

        // Calculate specular
        Vec2 view_dir = (to_render.camera_pos - nearest.location).normalized();
        Vec2 halfway = (view_dir + light_dir).normalized();
        int shininess = 16;
        float specular = pow(std::max(normal * halfway, 0.0f), shininess);
        color += Vec4(specular * 1, specular * 1, specular * 1, 0);
      }
    }
    return color;
  }
  else
  {
    return Vec4(0.1, 0.1, 0.1, 1);
  }
}

void
GraphicsServer::draw()
{
  glfwPollEvents();

  Vec2 viewport_size = get_framebuffer_size();
  glViewport(0, 0, int(viewport_size.x), int(viewport_size.y));
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (current_screen != nullptr)
    current_screen->draw_custom(this);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void
GraphicsServer::draw_color_rect(Vec2 origin, Vec2 size, Vec4 color)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  color_shader->bind_uniform(get_pixel_to_screen_transform()
    * Mat3::translate(origin)
    * Mat3::scale(size), "transform");
  color_shader->bind_uniform(color, "color");
  color_shader->draw(quad);
}

void
GraphicsServer::draw_texture_rect(Vec2 origin, Vec2 size, const Texture &texture)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  texture_shader->bind_uniform(get_pixel_to_screen_transform()
    * Mat3::translate(origin)
    * Mat3::scale(size), "transform");
  texture_shader->bind_uniform(texture, "sampler");
  texture_shader->draw(quad);
}

void
GraphicsServer::render_to_rect(Vec2 origin, Vec2 size, RenderRequest to_render)
{
  if (to_render.camera_mode == Vertical)
  {
    Vec2 unit_direction = to_render.camera_dir.normalized();
    Vec2 unit_perp = Vec2(-unit_direction.y, unit_direction.x);

    float row_height = size.y / 224.0;

    for (unsigned int i = 0; i < 224; ++i)
    {
      Vec2 ray_dir = unit_direction + (((float(i) - 112.0) / 112.0) * 0.5f * unit_perp);
      ray_dir = ray_dir.normalized();

      RayInfo cast = {};
      cast.origin = to_render.camera_pos;
      cast.direction = ray_dir;

      Vec4 color = render_ray(to_render, cast);

      color_shader->bind_uniform(get_pixel_to_screen_transform()
        * Mat3::translate(Vec2(origin.x, origin.y + (float(i) * row_height)))
        * Mat3::scale(Vec2(size.x, row_height)), "transform");
      color_shader->bind_uniform(color, "color");
      color_shader->draw(quad);
    }
  }
  else if (to_render.camera_mode == Horizontal)
  {
    Vec2 unit_direction = to_render.camera_dir.normalized();
    Vec2 unit_perp = Vec2(-unit_direction.y, unit_direction.x);

    float column_width = size.x / 256.0;

    for (unsigned int i = 0; i < 256; ++i)
    {
      Vec2 ray_dir = unit_direction + (((float(i) - 128.0) / 128.0) * 0.5f * unit_perp);
      ray_dir = ray_dir.normalized();

      RayInfo cast = {};
      cast.origin = to_render.camera_pos;
      cast.direction = ray_dir;

      Vec4 color = render_ray(to_render, cast);

      color_shader->bind_uniform(get_pixel_to_screen_transform()
        * Mat3::translate(Vec2(origin.x + (float(i) * column_width), origin.y))
        * Mat3::scale(Vec2(column_width, size.y)), "transform");
      color_shader->bind_uniform(color, "color");
      color_shader->draw(quad);
    }
  }
}
