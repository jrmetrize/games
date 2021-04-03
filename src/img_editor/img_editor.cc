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

}

}
