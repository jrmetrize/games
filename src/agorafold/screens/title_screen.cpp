#include "screens/title_screen.h"
#include "core/state.h"
#include "core/graphics.h"

TitleScreen::TitleScreen()
{
  listener.mouse_button_handle = std::bind(&TitleScreen::mouse_pressed, this,
    std::placeholders::_1, std::placeholders::_2);

  play_button = new MenuButton("Play", Vec2(0, 300), Vec2(0, 50),
    std::bind(&TitleScreen::button_pressed, this, 0));
  options_button = new MenuButton("Options", Vec2(0, 200), Vec2(0, 50),
    std::bind(&TitleScreen::button_pressed, this, 1));
  quit_button = new MenuButton("Quit", Vec2(0, 100), Vec2(0, 50),
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
  button->size.x = window_size.x;
  button->draw();
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
