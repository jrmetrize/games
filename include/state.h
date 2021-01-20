#ifndef STATE_H
#define STATE_H

class GraphicsServer;
class InputMonitor;

class LevelEditorScreen;
class LevelScreen;
class OptionsScreen;
class TitleScreen;

class Screen;

class GameState
{
  static GameState *instance;

  bool should_close;

  GraphicsServer *graphics_server;

  LevelEditorScreen *level_editor_screen;
  LevelScreen *level_screen;
  OptionsScreen *options_screen;
  TitleScreen *title_screen;

  Screen *current_screen;
public:
  GameState(GraphicsServer *_graphics_server);

  ~GameState();

  static void
  set_instance(GameState *_instance);

  static GameState *
  get();

  void
  update(InputMonitor *input, float time_elapsed);

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
