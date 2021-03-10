#include "graphics.h"
#include "screen.h"
#include "resource.h"
#include "glad/glad.h"
#include "backends/graphics_opengl.h"
#include "backends/graphics_vulkan.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GraphicsLayer::~GraphicsLayer()
{

}

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

void
GraphicsLayer::set_mesh_binding(Mesh *mesh, void *binding)
{
  mesh->binding = binding;
}

void *
GraphicsLayer::get_mesh_binding(const Mesh *mesh)
{
  return mesh->binding;
}

Vertex::Vertex(const Vec3 &_position, const Vec2 &_texture_coordinates) :
  position(_position),
  texture_coordinates(_texture_coordinates)
{

}

Mesh::Mesh(const VertexVector &_vertices, const IndexVector &_indices) :
  vertices(_vertices),
  indices(_indices)
{

}

Mesh::~Mesh()
{

}

const VertexVector &
Mesh::get_vertices() const
{
  return vertices;
}

const IndexVector &
Mesh::get_indices() const
{
  return indices;
}

Mesh *
Mesh::primitive_quad()
{
  VertexVector vertices = {
    Vertex(Vec3(0.0, 0.0, 0.0), Vec2(0.0, 1.0)),
    Vertex(Vec3(1.0, 0.0, 0.0), Vec2(1.0, 1.0)),
    Vertex(Vec3(0.0, 1.0, 0.0), Vec2(0.0, 0.0)),
    Vertex(Vec3(1.0, 1.0, 0.0), Vec2(1.0, 0.0))
  };

  IndexVector indices = {
    0, 1, 2,
    1, 2, 3
  };

  return new Mesh(vertices, indices);
}

Mesh *
Mesh::primitive_cube()
{
  VertexVector vertices = {
    {{-1, -1, -1}, {0, 0}},
    {{1, -1, -1}, {0, 0}},
    {{1, 1, -1}, {0, 0}},
    {{-1, 1, -1}, {0, 0}},
    {{-1, -1, 1}, {0, 0}},
    {{1, -1, 1}, {0, 0}},
    {{1, 1, 1}, {0, 0}},
    {{-1, 1, 1}, {0, 0}}
  };
  IndexVector indices = {
    0, 2, 1, /* -z */
    0, 3, 2,

    4, 5, 6, /* +z */
    4, 6, 7,

    0, 1, 5, /* -y */
    0, 5, 4,

    6, 3, 7, /* +y */
    6, 2, 3,

    0, 7, 3, /* -x */
    0, 4, 7,

    6, 5, 1, /* +x */
    6, 1, 2
  };

  return new Mesh(vertices, indices);
}

NoiseTexture::NoiseTexture(uint32_t _seed, float _frequency) :
  seed(_seed), noise(seed), frequency(_frequency)
{
  noise.SetFrequency(frequency);
}

Vec4
NoiseTexture::value_at(Vec2 x)
{
  return Vec4(noise.GetNoise(x.x, x.y));
}

Vec4
NoiseTexture::value_at(Vec3 x)
{
  return Vec4(noise.GetNoise(x.x, x.y, x.z));
}

void
NoiseTexture::set_frequency(float _frequency)
{
  frequency = _frequency;
  noise.SetFrequency(frequency);
}

Segment::Segment(Vec2 _p1, Vec2 _p2, Material *_material) :
  p1(_p1),
  p2(_p2),
  material(_material)
{

}

Material *
Segment::get_material() const
{
  return material;
}

Vec2
Segment::get_normal() const
{
  Vec2 dir = p2 - p1;
  Vec2 normal = Vec2(-dir.y, dir.x);
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

GraphicsServer * GraphicsServer::instance = nullptr;

GraphicsServer::GraphicsServer(GLFWwindow *_window) :
  window(_window),
  current_screen(nullptr)
{
  backend = new GraphicsLayerOpenGL();
  backend->set_graphics_server(this);

  quad = Mesh::primitive_quad();
  bind(quad);

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

void
GraphicsServer::bind(Texture *tex)
{
  // Bind the texture to the backend.
  backend->bind_texture(tex);
}

void
GraphicsServer::bind(Mesh *mesh)
{
  backend->bind_mesh(mesh);
}

Mesh *
GraphicsServer::get_quad()
{
  return quad;
}

void
GraphicsServer::set_fullscreen(bool fullscreen)
{
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
  Vec2 scale;
  glfwGetWindowContentScale(window, &scale.x, &scale.y);
  return scale;
}

Vec2
GraphicsServer::get_framebuffer_size(bool scaled) const
{
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
}

Vec4
GraphicsServer::render_ray(RenderRequest to_render, RayInfo ray)
{
  RayResult nearest;
  nearest.distance = -1;
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
    /*
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
    */
    Vec2 hit_location = nearest.location;
    if (hit->get_material() != nullptr)
      return hit->get_material()->light(ray.direction, hit_location, hit);
    else
      return Vec4(1);
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

  backend->begin_render();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (current_screen != nullptr)
    current_screen->draw_custom();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
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
      draw_color_rect(Vec2(origin.x, origin.y + (float(i) * row_height)),
        Vec2(size.x, row_height), color);
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
      draw_color_rect(Vec2(origin.x + (float(i) * column_width), origin.y),
        Vec2(column_width, size.y), color);
    }
  }
}
