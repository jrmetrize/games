#include "screens/title_screen.h"
#include "imgui/imgui.h"
#include "state.h"
#include "graphics.h"

TitleScreen::TitleScreen()
{
  listener.mouse_button_handle = std::bind(&TitleScreen::mouse_pressed, this,
    std::placeholders::_1, std::placeholders::_2);

  play_button = new MenuButton("Play", 300,
    std::bind(&TitleScreen::button_pressed, this, 0));
  options_button = new MenuButton("Options", 200,
    std::bind(&TitleScreen::button_pressed, this, 1));
  quit_button = new MenuButton("Quit", 100,
    std::bind(&TitleScreen::button_pressed, this, 2));
}

TitleScreen::~TitleScreen()
{
  delete play_button;
  delete options_button;
  delete quit_button;
}

void
TitleScreen::draw_button(MenuButton *button)
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  TextRenderRequest req = {};
  req.bounding_box_origin = Vec2(0, button->height);
  req.bounding_box_size = Vec2(window_size.x, 50);
  req.text = button->text;
  req.font = GameState::get()->get_serif();
  req.center = true;

  if (button->pressed)
  {
    req.size = 42;
    req.color = Vec4(0.5, 0.5, 0.5, 1);
  }
  else if (button->highlighted)
  {
    req.size = 42;
    float sine = sin(3.0f * GameState::get()->get_time());
    float brightness = 0.7f + (0.3f * sine);
    req.color = Vec4(brightness, brightness, brightness, 1);
  }
  else
  {
    req.size = 36;
    req.color = Vec4(1);
  }

  GraphicsServer::get()->draw_text(req);
}

void
TitleScreen::mouse_pressed(MouseButton button, bool pressed)
{
  play_button->mouse_pressed(button, pressed);
  options_button->mouse_pressed(button, pressed);
  quit_button->mouse_pressed(button, pressed);
}

void
TitleScreen::update(float time_elapsed)
{
  play_button->update();
  options_button->update();
  quit_button->update();
}

void
TitleScreen::to_appear()
{
  InputMonitor::get()->install_listener(&listener);
}

void
TitleScreen::to_disappear()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
TitleScreen::draw_custom()
{
  draw_button(play_button);
  draw_button(options_button);
  draw_button(quit_button);

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

void
TitleScreen::button_pressed(int button)
{
  if (button == 0)
    GameState::get()->switch_to_screen(GameState::get()->get_level_screen());
  else if (button == 1)
    GameState::get()->switch_to_screen(GameState::get()->get_options_screen());
  else if (button == 2)
    GameState::get()->close_game();
}
