#include "img_editor/img_editor.hh"

namespace ImgEditor
{

ColorSwatch::ColorSwatch(Vec3 _color, std::string _name) :
  color(_color), name(_name)
{

}

unsigned char
ColorSwatch::get_red() const
{
  return (unsigned char)(255.0f * color.x);
}

unsigned char
ColorSwatch::get_green() const
{
  return (unsigned char)(255.0f * color.y);
}

unsigned char
ColorSwatch::get_blue() const
{
  return (unsigned char)(255.0f * color.z);
}

unsigned char
ColorSwatch::get_alpha() const
{
  return 0xFF;
}

ColorPalette::ColorPalette() :
  colors()
{

}

ColorPalette::~ColorPalette()
{

}

const std::vector<ColorSwatch> &
ColorPalette::get_colors() const
{
  return colors;
}

std::vector<ColorSwatch> &
ColorPalette::get_colors()
{
  return colors;
}

/* http://eastfarthing.com/blog/2016-05-06-palette/ */
ColorPalette *
ColorPalette::make_32_palette()
{
  ColorPalette *p = new ColorPalette();

  p->colors = std::vector<ColorSwatch>({
    { Vec3(0.840f, 0.626f, 0.564f), "pinkish tan" },
    { Vec3(0.996f, 0.231f, 0.118f), "orangey red" },
    { Vec3(0.633f, 0.175f, 0.196f), "rouge" },
    { Vec3(0.980f, 0.185f, 0.477f), "strong pink" },
    { Vec3(0.982f, 0.624f, 0.855f), "bubblegum pink" },
    { Vec3(0.901f, 0.111f, 0.968f), "pink/purple" },
    { Vec3(0.599f, 0.186f, 0.487f), "warm purple" },
    { Vec3(0.279f, 0.500f, 0.125f), "burgundy" },
    { Vec3(0.023f, 0.067f, 0.333f), "navy blue" },
    { Vec3(0.312f, 0.900f, 0.923f), "blue/purple" },
    { Vec3(0.177f, 0.413f, 0.795f), "medium blue" },
    { Vec3(0.003f, 0.649f, 0.933f), "azure" },
    { Vec3(0.435f, 0.919f, 0.999f), "robin's egg" },
    { Vec3(0.032f, 0.636f, 0.605f), "blue/green" },
    { Vec3(0.167f, 0.401f, 0.415f), "dark aqua" },
    { Vec3(0.024f, 0.212f, 0.100f), "dark forest green" },
    { Vec3(0.000f, 0.000f, 0.000f), "black" },
    { Vec3(0.290f, 0.286f, 0.343f), "charcoal grey" },
    { Vec3(0.555f, 0.483f, 0.644f), "greyish purple" },
    { Vec3(0.717f, 0.753f, 0.612f), "greenish grey" },
    { Vec3(1.000f, 1.000f, 1.000f), "white" },
    { Vec3(0.674f, 0.744f, 0.612f), "greenish grey" },
    { Vec3(0.509f, 0.488f, 0.440f), "medium grey" },
    { Vec3(0.352f, 0.233f, 0.110f), "brown" },
    { Vec3(0.682f, 0.396f, 0.028f), "umber" },
    { Vec3(0.968f, 0.664f, 0.190f), "yellowish orange" },
    { Vec3(0.955f, 0.915f, 0.360f), "yellowish" },
    { Vec3(0.606f, 0.584f, 0.004f), "pea soup" },
    { Vec3(0.339f, 0.384f, 0.017f), "mud green" },
    { Vec3(0.070f, 0.588f, 0.231f), "kelley green" },
    { Vec3(0.317f, 0.880f, 0.076f), "toxic green" },
    { Vec3(0.035f, 0.991f, 0.798f), "bright teal" }
  });

  return p;
}

ImgEditorState::ImgEditorState()
{
  screen = new ImgEditorScreen();
}

ImgEditorState::~ImgEditorState()
{
  delete screen;
}

void
ImgEditorState::load()
{

}

void
ImgEditorState::start()
{
  EngineState::get()->switch_to_screen(screen);
}

void
ImgEditorState::close()
{

}

IVec2::IVec2(uint32_t _x, uint32_t _y) :
  x(_x), y(_y)
{

}

RawSprite::RawSprite(ColorPalette *_palette, IVec2 _size) :
  palette(_palette), size(_size)
{
  colors = new int32_t[size.x * size.y];
  for (uint32_t i = 0; i < size.x * size.y; ++i)
    colors[i] = 0; /* -1 indicates clear, non-negative values are for colors */
}

RawSprite::~RawSprite()
{
  delete colors;
}

IVec2
RawSprite::get_size() const
{
  return size;
}

void
RawSprite::set_size(IVec2 _size)
{
  size = _size;
}

ImgEditorScreen::ImgEditorScreen() :
  cursor_pos(0, 0), sprite(nullptr)
{
  ColorPalette *palette_32 = ColorPalette::make_32_palette();

  {
    unsigned char *palette_data = new unsigned char[8 * 4 * 4];

    for (uint32_t i = 0; i < 32; ++i)
    {
      ColorSwatch s = palette_32->get_colors()[i];
      palette_data[(4 * i) + 0] = s.get_red();
      palette_data[(4 * i) + 1] = s.get_green();
      palette_data[(4 * i) + 2] = s.get_blue();
      palette_data[(4 * i) + 3] = s.get_alpha();
    }

    palette = new Texture(8, 4, 4, palette_data);
    b_pal = GraphicsServer::get()->bind(palette);
    b_pal->set_filtering(Texture::Filtering::Nearest);

    delete palette_data;
  }

  sprite = new RawSprite(palette_32, IVec2(32, 32));

  input_test = new TextLine(Vec2(150, 100), Vec2(128, 24), "test");
  add_control(input_test);
  set_active_control(input_test);
}

ImgEditorScreen::~ImgEditorScreen()
{

}

void
ImgEditorScreen::key_update(Key key, bool pressed)
{
  if (pressed)
  {
    /* Handle released keys. These should probably be their own methods. */
    /* Maybe do some stuff with modifier keys... e.g. holding control makes
       move keys work in increments of 4? */
    if (key == Key::KeyLeft)
    {
      if (cursor_pos.x > 0)
        cursor_pos.x -= 1;
    }
    else if (key == Key::KeyRight)
    {
      if (cursor_pos.x < sprite->get_size().x - 1)
        cursor_pos.x += 1;
    }
    else if (key == Key::KeyDown)
    {
      if (cursor_pos.y > 0)
        cursor_pos.y -= 1;
    }
    else if (key == Key::KeyUp)
    {
      if (cursor_pos.y < sprite->get_size().y - 1)
        cursor_pos.y += 1;
    }
  }
}

void
ImgEditorScreen::to_appear()
{

}

void
ImgEditorScreen::to_disappear()
{

}

void
ImgEditorScreen::update(float time_elapsed)
{
  input_test->update(time_elapsed);
}

void
ImgEditorScreen::draw()
{
  draw_controls();
#if 0
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();

  GraphicsServer::get()->draw_color_rect(Vec2(), window_size, IMG_EDITOR_BG_COLOR);

  GraphicsServer::get()->draw_texture_rect(Vec2(0, 32), Vec2(128), *b_pal);

  /* Draw the info bar */
  std::string infobar_text = "";
  infobar_text += "Image Size: (" + std::to_string(sprite->get_size().x) + ", " +
    std::to_string(sprite->get_size().y) + "), ";
  infobar_text += "Cursor Pos: (" + std::to_string(cursor_pos.x) + ", " +
    std::to_string(cursor_pos.y) + ").";

  TextRenderRequest info_text = {};
  info_text.bounding_box_origin = Vec2(0, 0);
  info_text.bounding_box_size = Vec2(window_size.x, 32.0f);
  info_text.text = infobar_text;
  info_text.color = Vec4(1);
  info_text.size = 12.0f;
  info_text.font = EngineState::get()->get_serif();
  info_text.center = false;
  info_text.center_vertical = false;
  GraphicsServer::get()->draw_text(info_text);

  /* Draw the image in the rectangle */
  Vec2 margin_size = Vec2(512, 512) + Vec2(32, 32);
  GraphicsServer::get()->draw_color_rect((0.5 * window_size) - (0.5 * margin_size),
    margin_size, IMG_EDITOR_IMG_BG);

  Vec2 canvas_size = Vec2(512, 512);
  GraphicsServer::get()->draw_color_rect((0.5 * window_size) - (0.5 * canvas_size)
    + Vec2(float(cursor_pos.x) * (canvas_size.x / sprite->get_size().x),
      float(cursor_pos.y) * (canvas_size.y / sprite->get_size().y)),
    Vec2(canvas_size.x / sprite->get_size().x, canvas_size.y / sprite->get_size().y),
    Vec4(1));
  /*
  GraphicsServer::get()->draw_texture_rect((0.5 * window_size) - (0.5 * canvas_size),
    canvas_size, b_sprite);
  */

  /* Draw the usage guide. */
  {
    /* TODO: put this string in a resource bundle so localization is possible */
    TextRenderRequest usage_text = {};
    usage_text.bounding_box_origin = Vec2(0, 128.0f);
    usage_text.bounding_box_size = Vec2(320.0f, window_size.y - 128.0f);
    usage_text.text = "Arrow Keys: position cursor on canvas\nWASD: position cursor on palette\nX: clear pixel\nSpace: set pixel color";
    usage_text.color = Vec4(1);
    usage_text.size = 12.0f;
    usage_text.font = EngineState::get()->get_serif();
    usage_text.center = false;
    usage_text.center_vertical = false;
    GraphicsServer::get()->draw_text(usage_text);
  }

  //input_test->draw();
#endif
}

}
