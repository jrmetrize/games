#include "img_editor/img_editor.hh"

namespace ImgEditor
{

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
    unsigned char *palette_data = new unsigned char[8 * 8 * 4];

    palette = new Texture(8, 8, 4, palette_data);

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
  //GraphicsServer::get()->draw_texture_rect(Vec2(), Vec2(128), *palette);
}

}
