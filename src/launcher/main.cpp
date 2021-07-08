#include <string>
#include <chrono>

#include <core/audio.h>
#include <core/graphics.h>
#include <core/input.h>
#include <core/state.h>

#include "launcher/game_select.h"
#include "launcher/launcher.h"
#include "launcher/title.h"

#include "resource_editor/resource_editor.hh"

#include "z2d/z2d.hh"

using namespace Launcher;

LauncherGame::~LauncherGame()
{

}

class LauncherStateImpl : public LauncherState
{
  LauncherGame *current_game;

  TitleScreen *title_screen;
  GameSelectScreen *select_screen;
public:
  LauncherStateImpl()
  {
    select_screen = new GameSelectScreen(this);
    title_screen = new TitleScreen(this);
  }

  ~LauncherStateImpl()
  {
    delete select_screen;
    delete title_screen;
  }

  void
  show_title_screen()
  {
    EngineState::get()->switch_to_screen(title_screen);
  }

  void
  show_game_select_screen()
  {
    EngineState::get()->switch_to_screen(select_screen);
  }

  void
  launch_game(std::string game)
  {
    /* For now, just launch the game instead of going to a detail page. */
    /* TODO: Instead of a dumb if statement, do this smarter... maybe
       have a 'launch' function in each entry. */
    if (game == "resource_editor")
    {
      current_game = new ResourceEditor::ResourceEditorState();
    }
    else if (game == "z2d")
    {
      current_game = new Z2D::Z2DState();
    }
    current_game->load();
    current_game->start();
  }
};

int
main(int argc, const char **argv)
{
  // TODO: we need to be much more careful about dependencies here. Ideally,
  // none of the singletons depend on each other in constructors, and
  // proper initialization with dependencies will happend after all singletons
  // have been created.
  GraphicsServer *renderer = new GraphicsServer();
  GraphicsServer::set_instance(renderer);

  InputMonitor *input = new InputMonitor(renderer->get_window());
  InputMonitor::set_instance(input);

  EngineState *state = new EngineState();
  EngineState::set_instance(state);

  /*
  AudioServer *audio = new AudioServer();
  AudioServer::set_instance(audio);
  audio->start();
  */

  LauncherStateImpl *launcher = new LauncherStateImpl();
  launcher->show_title_screen();

  std::chrono::time_point<std::chrono::steady_clock> last_frame =
    std::chrono::steady_clock::now();

  while (state->game_open())
  {
    std::chrono::time_point<std::chrono::steady_clock> current_frame =
      std::chrono::steady_clock::now();
    float duration =
      float(std::chrono::duration_cast<std::chrono::microseconds>(current_frame
      - last_frame).count()) / (1000 * 1000);
    last_frame = current_frame;
    state->update(duration);
    renderer->draw();
  }

  delete launcher;

  //delete audio;
  delete renderer;
  delete state;
  delete input;

  return 0;
}
