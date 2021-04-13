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
  GameState::get()->switch_to_screen(screen);
}

void
ImgEditorState::close()
{

}

ImgEditorScreen::ImgEditorScreen()
{
  {
    unsigned char *palette_data = new unsigned char[8 * 4 * 4];
    ColorPalette *palette_32 = ColorPalette::make_32_palette();

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
}

ImgEditorScreen::~ImgEditorScreen()
{

}

void
ImgEditorScreen::update(float time_elapsed)
{

}

void
ImgEditorScreen::draw()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();

  GraphicsServer::get()->draw_texture_rect(Vec2(0, 32), Vec2(128), *b_pal);

  /* Draw the info bar */
  std::string infobar_text = "";
  infobar_text += "Image Size: (32, 32), ";
  infobar_text += "Cursor Pos: (-, -).";

  TextRenderRequest info_text = {};
  info_text.bounding_box_origin = Vec2(0, 0);
  info_text.bounding_box_size = Vec2(window_size.x, 32.0f);
  info_text.text = infobar_text;
  info_text.color = Vec4(1);
  info_text.size = 12.0f;
  info_text.font = GameState::get()->get_serif();
  info_text.center = false;
  info_text.center_vertical = false;
  GraphicsServer::get()->draw_text(info_text);
}

}
