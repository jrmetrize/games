#include "state.h"
#include "graphics.h"
#include "screens/level_editor_screen.h"
#include "screens/level_screen.h"
#include "screens/options_screen.h"
#include "screens/title_screen.h"

GameState * GameState::instance = nullptr;

GameState::GameState(GraphicsServer *_graphics_server) :
  should_close(false),
  graphics_server(_graphics_server),
  current_screen(nullptr)
{
  level_editor_screen = new LevelEditorScreen(this);
  level_screen = new LevelScreen(this);
  options_screen = new OptionsScreen(this);
  title_screen = new TitleScreen(this);

  graphics_server->set_current_screen(title_screen);
  current_screen = title_screen;
}

GameState::~GameState()
{
  delete level_editor_screen;
  delete level_screen;
  delete options_screen;
  delete title_screen;
}

void
GameState::set_instance(GameState *_instance)
{
  instance = _instance;
}

GameState *
GameState::get()
{
  return instance;
}

void
GameState::update(InputMonitor *input, float time_elapsed)
{
  current_screen->update(input, time_elapsed);
}

void
GameState::title_screen_to_level_screen()
{
  graphics_server->set_current_screen(level_screen);
  current_screen = level_screen;
}

void
GameState::level_screen_to_title_screen()
{
  graphics_server->set_current_screen(title_screen);
  current_screen = title_screen;
}

void
GameState::title_screen_to_level_editor_screen()
{
  graphics_server->set_current_screen(level_editor_screen);
  current_screen = level_editor_screen;
}

void
GameState::level_editor_screen_to_title_screen()
{
  graphics_server->set_current_screen(title_screen);
  current_screen = title_screen;
}

void
GameState::title_screen_to_options_screen()
{
  graphics_server->set_current_screen(options_screen);
  current_screen = options_screen;
}

void
GameState::options_screen_to_title_screen()
{
  graphics_server->set_current_screen(title_screen);
  current_screen = title_screen;
}

void
GameState::close_game()
{
  should_close = true;
}

bool
GameState::game_open() const
{
  return !should_close;
}
