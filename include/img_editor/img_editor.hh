#ifndef IMG_EDITOR_HH
#define IMG_EDITOR_HH

#include <launcher/launcher.h>
#include <core/screen.h>
#include <core/graphics.h>

#define IMG_EDITOR_BG_COLOR Vec4(0.3, 0.3, 0.3, 1.0)
#define IMG_EDITOR_IMG_BG Vec4(0.1, 0.1, 0.1, 1.0)

namespace ImgEditor
{

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

/* TODO: integrate this with linear algebra code. Ideally, the vector
   class should be a generic type for an element of a free module over
   a ring provided as the template argument. */
struct IVec2
{
  uint32_t x;
  uint32_t y;

  IVec2(uint32_t _x, uint32_t _y);
};

/* Container for sprite data *for editing*. For example, colors are stored
   as references to a palette. This is not the format to use for using a sprite
   in a game. */
class RawSprite
{
  const ColorPalette *palette;
  IVec2 size;

  /* TODO: add animation frames and stuff */
  int32_t *colors;
public:
  RawSprite(ColorPalette *_palette, IVec2 _size);

  ~RawSprite();

  IVec2
  get_size() const;

  /* TODO: include an option for how to perform the resize, such as scaling vs
     truncation, etc. */
  void
  set_size(IVec2 _size);
};

class ImgEditorScreen : public Screen
{
  Texture *palette;
  BoundTexture *b_pal;

  RawSprite *sprite;
  BoundTexture *b_sprite;

  Listener listener;

  IVec2 cursor_pos;

  void
  key_pressed(Key key, bool pressed);
public:
  ImgEditorScreen();

  ~ImgEditorScreen();

  void
  to_appear();

  void
  to_disappear();

  void
  update(float time_elapsed);

  void
  draw();
};

}

#endif
