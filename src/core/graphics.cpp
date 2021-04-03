#include "core/graphics.h"
#include "core/screen.h"
#include "core/resource.h"
#include "core/backends/graphics_opengl.h"
#include "core/backends/graphics_vulkan.h"

Texture::Texture(unsigned int _width, unsigned int _height, unsigned int _channels,
  const unsigned char *_data) :
  width(_width), height(_height), channels(_channels), data(_data)
{
  GraphicsServer::get()->bind(this);
}

Texture::~Texture()
{

}

unsigned int
Texture::get_width() const
{
  return width;
}

unsigned int
Texture::get_height() const
{
  return height;
}

unsigned int
Texture::get_channels() const
{
  return channels;
}

const unsigned char *
Texture::get_data() const
{
  return data;
}

BoundMesh::~BoundMesh()
{

}

Camera::Camera() :
  fovy(3.14159f / 3.0f), aspect_ratio(16.0f / 9.0f), clip_near(0.1f), clip_far(100.0f),
  position(), direction(Vec3(1, 0, 0))
{

}

void
Camera::set_fovy(float _fovy)
{
  fovy = _fovy;
}

void
Camera::set_aspect_ratio(float _aspect_ratio)
{
  aspect_ratio = _aspect_ratio;
}

void
Camera::set_clip_near(float _clip_near)
{
  clip_near = _clip_near;
}

void
Camera::set_clip_far(float _clip_far)
{
  clip_far = _clip_far;
}

Vec3
Camera::get_position() const
{
  return position;
}

void
Camera::set_position(Vec3 _position)
{
  position = _position;
}

void
Camera::set_direction(Vec3 _direction)
{
  direction = _direction;
}

Mat4
Camera::get_view_projection_matrix() const
{
  Mat4 projection = Mat4::projection(fovy, aspect_ratio, clip_near, clip_far);
  Mat4 view = Mat4::lookat(position, position + direction, Vec3(0, 1, 0));
  return projection * view;
}

SceneObject::SceneObject()
{
  transform = Mat4::identity();
}

Scene3D::Scene3D(Camera *_camera) :
  camera(_camera), ambient_color(), objects(), lights()
{

}

Scene3D::~Scene3D()
{

}

const Camera *
Scene3D::get_camera() const
{
  return camera;
}

Vec3
Scene3D::get_ambient_color() const
{
  return ambient_color;
}

void
Scene3D::set_ambient_color(Vec3 _ambient_color)
{
  ambient_color = _ambient_color;
}

std::vector<SceneObject *> &
Scene3D::get_objects()
{
  return objects;
}

const std::vector<SceneObject *> &
Scene3D::get_objects() const
{
  return objects;
}

std::vector<DirectionalLight *> &
Scene3D::get_lights()
{
  return lights;
}

const std::vector<DirectionalLight *> &
Scene3D::get_lights() const
{
  return lights;
}

void
GraphicsLayer::set_texture_binding(Texture *tex, void *binding)
{
  tex->binding = binding;
}

void *
GraphicsLayer::get_texture_binding(const Texture *tex)
{
  return tex->binding;
}

GraphicsLayer::~GraphicsLayer()
{

}

GraphicsServer * GraphicsServer::instance = nullptr;

GraphicsServer::GraphicsServer() :
  current_screen(nullptr)
{
  backend = new GraphicsLayerOpenGL();
  backend->set_graphics_server(this);

  quad = bind(Mesh::primitive_quad());
}

GraphicsServer::~GraphicsServer()
{
  delete quad;
}

void
GraphicsServer::set_instance(GraphicsServer *_instance)
{
  instance = _instance;
}

GraphicsServer *
GraphicsServer::get()
{
  return instance;
}

GLFWwindow *
GraphicsServer::get_window()
{
  return backend->get_window();
}

void
GraphicsServer::bind(Texture *tex)
{
  // Bind the texture to the backend.
  backend->bind_texture(tex);
}

BoundMesh *
GraphicsServer::bind(Mesh *mesh)
{
  return backend->bind_mesh(mesh);
}

BoundMesh *
GraphicsServer::get_quad()
{
  return quad;
}

void
GraphicsServer::set_fullscreen(bool fullscreen)
{
  GLFWwindow *window = backend->get_window();
  if (fullscreen)
  {
    int monitor_count;
    int monitor_width;
    int monitor_height;
    GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
    glfwGetMonitorWorkarea(monitors[0], nullptr, nullptr, &monitor_width, &monitor_height);

    glfwSetWindowMonitor(window, monitors[0], 0, 0, monitor_width, monitor_height, 144);
  }
  else
  {
    glfwSetWindowMonitor(window, nullptr, 64, 64, 1280, 720, 0);
  }
}

Vec2
GraphicsServer::get_scale() const
{
  GLFWwindow *window = backend->get_window();
  Vec2 scale;
  glfwGetWindowContentScale(window, &scale.x, &scale.y);
  return scale;
}

void
GraphicsServer::window_resize(Vec2 size)
{
  backend->window_resize(size);
  if (current_screen != nullptr)
    current_screen->resize(size);
}

Vec2
GraphicsServer::get_framebuffer_size(bool scaled) const
{
  GLFWwindow *window = backend->get_window();
  int width;
  int height;
  glfwGetFramebufferSize(window, &width, &height);

  if (scaled)
  {
    Vec2 scale = get_scale();

    return Vec2(float(width) / scale.x, float(height) / scale.y);
  }
  else
  {
    return Vec2(float(width), float(height));
  }
}

Mat3
GraphicsServer::get_pixel_to_screen_transform() const
{
  Vec2 viewport_size = get_framebuffer_size();
  Vec2 scale = get_scale();
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
  if (current_screen != nullptr)
    current_screen->resize(get_framebuffer_size());
}

void
GraphicsServer::draw()
{
  glfwPollEvents();

  backend->begin_render();

  if (current_screen != nullptr)
    current_screen->draw();

  glfwSwapBuffers(backend->get_window());
}

void
GraphicsServer::draw_color_rect(Vec2 origin, Vec2 size, Vec4 color)
{
  backend->draw_color_rect(origin, size, color);
}

void
GraphicsServer::draw_texture_rect(Vec2 origin, Vec2 size, const Texture &texture)
{
  backend->draw_texture_rect(origin, size, texture);
}

// Draws a line of text with no wrapping or alignment, with the baseline and
// start of the line at the origin of the bounding box specified
void
GraphicsServer::draw_text_line(const TextRenderRequest &text_request)
{
  // TODO: this should be calculated by the font face object
  float scale_factor = text_request.size / (64.0f * 64.0f);
  float texture_padding = 8; // 'spread' value in SDF generation

  text_request.font->generate_textures();
  Vec2 current_pos = text_request.bounding_box_origin;

  if (text_request.center_vertical)
  {
    const Glyph &glyph = text_request.font->get_glyph('B');
    float g_height = (scale_factor * glyph.height);

    // We want the vertical center of the text to be the same as the vertical
    // center of the bounding box.
    current_pos.y = text_request.bounding_box_origin.y
      + ((1.0f / 2.0f) * text_request.bounding_box_size.y)
      - ((1.0f / 2.0f) * g_height);
  }

  if (text_request.center)
  {
    float width = 0;
    for (unsigned int i = 0; i < text_request.text.length(); ++i)
    {
      char c = text_request.text[i];
      const Glyph &glyph = text_request.font->get_glyph(c);
      width += scale_factor * glyph.horizontal_advance;
    }
    current_pos += Vec2((1.0f / 2.0f) * (text_request.bounding_box_size.x - width), 0);
  }

  for (unsigned int i = 0; i < text_request.text.length(); ++i)
  {
    char c = text_request.text[i];
    const Glyph &glyph = text_request.font->get_glyph(c);
    const Texture *tex = text_request.font->get_texture(c);

    // We are scaling (glyph.bitmap_height - (2 * texture_padding)) bitmap pixels
    // to be scale_factor * glyph.height visible pixels tall
    float size_scale = (scale_factor * glyph.height) /
      (float(glyph.bitmap_height) - (2 * texture_padding));
    Vec2 glyph_size = size_scale * Vec2(glyph.bitmap_width,
      glyph.bitmap_height);

    // We also have to displace the origin of the quad by (-8, -8) bitmap pixels
    FontFace::Kerning kern = {};
    if (i > 0)
      kern = text_request.font->get_kerning(text_request.text[i - 1], c);
    Vec2 adjustment = scale_factor * Vec2(glyph.horizontal_bearing_x + kern.x,
      -glyph.height + glyph.horizontal_bearing_y + kern.y);
    adjustment += -scale_factor * Vec2(texture_padding * (float(glyph.width) / float(glyph.bitmap_width)),
      texture_padding * (float(glyph.height) / float(glyph.bitmap_height)));

    backend->draw_character(current_pos + adjustment, glyph_size,
      text_request.color, *tex);

    current_pos += scale_factor * Vec2(glyph.horizontal_advance, 0);
  }
}

void
GraphicsServer::draw_text(const TextRenderRequest &text_request)
{
  // TODO: this should be calculated by the font face object
  float scale_factor = text_request.size / (64.0f * 64.0f);
  float texture_padding = 8; // 'spread' value in SDF generation

  // TODO: there should be a mechanism to cache and/or precompute the calculations below
  // In order to arrange the lines, break the string into words (separated
  // by whitespace) and calculate the width of each word. Then greedily
  // add words to a line until the length of the bounding box will be exceeded.
  std::vector<std::string> words;
  std::string current_token = "";
  for (const char &c : text_request.text)
  {
    if (isspace(c) && current_token.length() > 0)
    {
      words.push_back(current_token);
      current_token = "";
    }
    else
    {
      current_token.append(1, c);
    }
  }
  if (current_token.length() > 0)
    words.push_back(current_token);
  std::vector<float> widths;
  for (unsigned int i = 0; i < words.size(); ++i)
  {
    float width = 0;
    for (unsigned int j = 0; j < words[i].length(); ++j)
    {
      char c = words[i][j];
      const Glyph &glyph = text_request.font->get_glyph(c);
      width += scale_factor * glyph.horizontal_advance;
    }
    widths.push_back(width);
  }
  float space_width =
    scale_factor * text_request.font->get_glyph(' ').horizontal_advance;
  std::vector<std::string> lines;
  std::vector<float> line_widths;
  std::string current_line = "";
  float current_line_width = 0;
  for (unsigned int i = 0; i < words.size(); ++i)
  {
    // Always add a word to a newline to prevent infinite loops
    if (current_line.length() == 0)
    {
      current_line = words[i];
      current_line_width = widths[i];
    }
    else
    {
      if (current_line_width + space_width + widths[i] > text_request.bounding_box_size.x)
      {
        // We can't add another word to this line, so add it to the list and start the next
        lines.push_back(current_line);
        line_widths.push_back(current_line_width);
        current_line = words[i];
        current_line_width = widths[i];
      }
      else
      {
        current_line.append(" " + words[i]);
        current_line_width += space_width + widths[i];
      }
    }
  }
  if (current_line.length() > 0)
  {
    lines.push_back(current_line);
    line_widths.push_back(current_line_width);
  }

  // Calculations are finally done, so render the lines
  for (unsigned int i = 0; i < lines.size(); ++i)
  {
    TextRenderRequest req = text_request;
    req.text = lines[i];
    req.center = false;

    Vec2 alignment = Vec2(0, text_request.bounding_box_size.y);
    if (text_request.center)
      alignment += Vec2((1.0f / 2.0f) * (text_request.bounding_box_size.x - line_widths[i]), 0);

    // TODO: Be able to adjust default leading (1.2)?
    req.bounding_box_origin = text_request.bounding_box_origin
      + alignment
      - Vec2(0, float(i + 1) * 1.2f * text_request.size);
    draw_text_line(req);
  }
}

void
GraphicsServer::clear_stencil_buffer()
{
  backend->clear_mask();
}

void
GraphicsServer::draw_stencil_rect(Vec2 origin, Vec2 size)
{
  backend->mask_rect(origin, size);
}

void
GraphicsServer::draw_3d(const Render3DRequest &scene_request)
{
  backend->draw_3d(scene_request);
}
