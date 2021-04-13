#ifndef IMG_EDITOR_HH
#define IMG_EDITOR_HH

#include <launcher/launcher.h>
#include <core/screen.h>
#include <core/graphics.h>

namespace ImgEditor
{

/* TODO: This should probably just be integrated into core. Also, do stuff
   with different color spaces, gamma correction, etc. */

struct ColorSwatch
{
  Vec3 color;
  std::string name;

  ColorSwatch(Vec3 _color, std::string _name);

  unsigned char
  get_red() const;

  unsigned char
  get_green() const;

  unsigned char
  get_blue() const;

  unsigned char
  get_alpha() const;
};

class ColorPalette
{
  std::vector<ColorSwatch> colors;
public:
  ColorPalette();

  ~ColorPalette();

  const std::vector<ColorSwatch> &
  get_colors() const;

  std::vector<ColorSwatch> &
  get_colors();

  static ColorPalette *
  make_32_palette();
};

class ImgEditorScreen;

class ImgEditorState : public Launcher::LauncherGame
{
  ImgEditorScreen *screen;
public:
  ImgEditorState();

  ~ImgEditorState();

  void
  load();

  void
  start();

  void
  close();
};

class ImgEditorScreen : public Screen
{
  Texture *palette;
  BoundTexture *b_pal;
public:
  ImgEditorScreen();

  ~ImgEditorScreen();

  void
  update(float time_elapsed);

  void
  draw();
};

}

#endif
