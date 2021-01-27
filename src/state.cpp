#include "state.h"
#include "graphics.h"
#include "input.h"
#include "resource.h"
#include "util.h"
#include "screens/level_editor_screen.h"
#include "screens/level_screen.h"
#include "screens/options_screen.h"
#include "screens/splash_screen.h"
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

const std::vector<DialogueChoice> &
DialoguePoint::get_choices() const
{
  return choices;
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

void
DialogueTree::choice_made(unsigned int index)
{
  current_point = points[current_point].get_choices()[index].next;
}

GameState * GameState::instance = nullptr;

GameState::GameState() :
  should_close(false),
  current_screen(nullptr),
  ref(std::chrono::steady_clock::now()),
  last_update(ref)
{
  level_editor_screen = new LevelEditorScreen();
  level_screen = new LevelScreen();
  options_screen = new OptionsScreen();
  splash_screen = new SplashScreen();
  title_screen = new TitleScreen();

  font_bundle = new ResourceBundle(local_to_absolute_path("resources/fonts.rbz"));

  current_screen = splash_screen;
  current_screen->to_appear();
}

GameState::~GameState()
{
  delete level_editor_screen;
  delete level_screen;
  delete options_screen;
  delete splash_screen;
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
GameState::update(float time_elapsed)
{
  last_update = std::chrono::steady_clock::now();
  GraphicsServer::get()->set_current_screen(current_screen);
  current_screen->update(time_elapsed);
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

Screen *
GameState::get_level_editor_screen()
{
  return level_editor_screen;
}

Screen *
GameState::get_level_screen()
{
  return level_screen;
}

Screen *
GameState::get_options_screen()
{
  return options_screen;
}

Screen *
GameState::get_splash_screen()
{
  return splash_screen;
}

Screen *
GameState::get_title_screen()
{
  return title_screen;
}

void
GameState::switch_to_screen(Screen *target_screen)
{
  current_screen->to_disappear();
  target_screen->to_appear();

  GraphicsServer::get()->set_current_screen(target_screen);
  current_screen = target_screen;
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
