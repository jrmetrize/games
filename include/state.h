#ifndef STATE_H
#define STATE_H

class GraphicsServer;

class LevelEditorScreen;
class LevelScreen;
class OptionsScreen;
class TitleScreen;

class GameState
{
  bool should_close;

  GraphicsServer *graphics_server;

  LevelEditorScreen *level_editor_screen;
  LevelScreen *level_screen;
  OptionsScreen *options_screen;
  TitleScreen *title_screen;
public:
  GameState(GraphicsServer *_graphics_server);

  ~GameState();

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
