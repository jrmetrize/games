#include "screens/title_screen.h"
#include "imgui/imgui.h"
#include "state.h"
#include "graphics.h"

TitleScreen::TitleScreen()
{
  listener.mouse_button_handle = std::bind(&TitleScreen::mouse_pressed, this,
    std::placeholders::_1, std::placeholders::_2);
  InputMonitor::get()->install_listener(&listener);
}

TitleScreen::~TitleScreen()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
TitleScreen::draw_button(float y, std::string text)
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  TextRenderRequest req = {};
  req.bounding_box_origin = Vec2(0, y);
  req.bounding_box_size = Vec2(window_size.x, 50);
  req.text = text;
  req.color = Vec4(1);
  req.size = 36;
  req.font = GameState::get()->get_serif();
  req.center = true;

  GraphicsServer::get()->draw_text(req);
}

void
TitleScreen::mouse_pressed(MouseButton button, bool pressed)
{
  //
}

void
TitleScreen::update(float time_elapsed)
{

}

void
TitleScreen::draw_custom()
{
  draw_button(300, "Play");
  draw_button(200, "Options");
  draw_button(100, "Quit");

  // TODO: actually render the menu through the engine and not imgui
  ImGui::Begin("Title Screen Menu");

  if (ImGui::Button("Play"))
  {
    GameState::get()->switch_to_screen(GameState::get()->get_level_screen());
  }

  if (ImGui::Button("Level Editor"))
  {
    GameState::get()->switch_to_screen(GameState::get()->get_level_editor_screen());
  }

  if (ImGui::Button("Options"))
  {
    GameState::get()->switch_to_screen(GameState::get()->get_options_screen());
  }

  if (ImGui::Button("Quit"))
  {
    GameState::get()->close_game();
  }

  ImGui::End();
}
