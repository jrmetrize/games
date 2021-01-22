#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>
#include <map>
#include <chrono>

class LevelEditorScreen;
class LevelScreen;
class OptionsScreen;
class TitleScreen;

class Screen;

class ResourceBundle;
class FontFace;

struct DialogueChoice
{
  std::string text;
  std::string next;
};

class DialoguePoint
{
  std::string text;
  std::string next;
  std::vector<DialogueChoice> choices;
public:
  DialoguePoint();

  DialoguePoint(std::string _text, std::string _next);

  void
  add_choice(const DialogueChoice &choice);

  std::string
  get_text() const;

  const std::vector<DialogueChoice> &
  get_choices() const;
};

class DialogueTree
{
  std::map<std::string, DialoguePoint> points;
  std::string current_point;
public:
  DialogueTree();

  void
  add_point(std::string name, const DialoguePoint &point);

  void
  set_current(std::string current);

  const DialoguePoint &
  get_current_point();

  void
  choice_made(unsigned int index);
};

class GameState
{
  std::chrono::time_point<std::chrono::steady_clock> ref;
  std::chrono::time_point<std::chrono::steady_clock> last_update;

  static GameState *instance;

  bool should_close;

  LevelEditorScreen *level_editor_screen;
  LevelScreen *level_screen;
  OptionsScreen *options_screen;
  TitleScreen *title_screen;

  Screen *current_screen;

  ResourceBundle *font_bundle;
public:
  GameState();

  ~GameState();

  static void
  set_instance(GameState *_instance);

  static GameState *
  get();

  void
  update(float time_elapsed);

  float
  get_time() const;

  FontFace *
  get_sans();

  FontFace *
  get_serif();

  void
  title_screen_to_level_screen();

  void
  level_screen_to_title_screen();

  void
  title_screen_to_level_editor_screen();

  void
  level_editor_screen_to_title_screen();

  void
  title_screen_to_options_screen();

  void
  options_screen_to_title_screen();

  void
  close_game();

  bool
  game_open() const;
};

#endif
