#ifndef IMAGE_EDITOR_HH
#define IMAGE_EDITOR_HH

#include <core/screen.h>
#include <core/graphics.h>

namespace ResourceEditor
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

  class SpriteView : public MenuControl
  {
    const BoundTexture *sprite;
  public:
    SpriteView(Vec2 _origin, Vec2 _size, const BoundTexture *_sprite);

    ~SpriteView();

    void
    update(float time_elapsed);

    void
    draw();

    void
    mouse_pressed(MouseButton button, bool button_pressed);
  };

  class ImgEditorScreen : public Screen
  {
    Texture *palette;
    BoundTexture *b_pal;

    RawSprite *sprite;
    BoundTexture *b_sprite;

    IVec2 cursor_pos;

    FlexContainer *color_container;
    ColorSelector *color_input;

    FlexContainer *palette_container;
    SpriteView *palette_view;

    FlexContainer *img_container;
    SpriteView *img_view;
  protected:
    void
    key_update(Key key, bool pressed);
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
