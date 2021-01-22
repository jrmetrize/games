#include "state.h"
#include "graphics.h"
#include "resource.h"
#include "util.h"
#include "screens/level_editor_screen.h"
#include "screens/level_screen.h"
#include "screens/options_screen.h"
#include "screens/title_screen.h"

DialoguePoint::DialoguePoint()
{

}

DialoguePoint::DialoguePoint(std::string _text, std::string _next) :
  text(_text), next(_next), choices()
{

}

void
DialoguePoint::add_choice(const DialogueChoice &choice)
{
  choices.push_back(choice);
}

std::string
DialoguePoint::get_text() const
{
  return text;
}

DialogueTree::DialogueTree() :
  points(), current_point()
{

}

void
DialogueTree::add_point(std::string name, const DialoguePoint &point)
{
  points[name] = point;
}

void
DialogueTree::set_current(std::string current)
{
  current_point = current;
}

const DialoguePoint &
DialogueTree::get_current_point()
{
  return points[current_point];
}

GameState * GameState::instance = nullptr;

GameState::GameState(GraphicsServer *_graphics_server) :
  should_close(false),
  graphics_server(_graphics_server),
  current_screen(nullptr),
  ref(std::chrono::steady_clock::now()),
  last_update(ref)
{
  level_editor_screen = new LevelEditorScreen(this);
  level_screen = new LevelScreen(this);
  options_screen = new OptionsScreen(this);
  title_screen = new TitleScreen(this);

  font_bundle = new ResourceBundle(local_to_absolute_path("resources/fonts.rbz"));

  graphics_server->set_current_screen(title_screen);
  current_screen = title_screen;
}

GameState::~GameState()
{
  delete level_editor_screen;
  delete level_screen;
  delete options_screen;
  delete title_screen;

  delete font_bundle;
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
  last_update = std::chrono::steady_clock::now();
  current_screen->update(input, time_elapsed);
}

float
GameState::get_time() const
{
  uint32_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
    last_update - ref
  ).count();
  return float(milliseconds) / 1000.0f;
}

FontFace *
GameState::get_sans()
{
  return reinterpret_cast<FontFace *>(font_bundle->get_resource("sans"));
}

FontFace *
GameState::get_serif()
{
  return reinterpret_cast<FontFace *>(font_bundle->get_resource("serif"));
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
