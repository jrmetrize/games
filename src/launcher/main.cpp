#include <string>
#include <chrono>

#include "core/audio.h"
#include "core/graphics.h"
#include "core/input.h"
#include "core/state.h"

#include "launcher/game_select.h"

using namespace Launcher;

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

  GameState *state = new GameState();
  GameState::set_instance(state);

  /*
  AudioServer *audio = new AudioServer();
  AudioServer::set_instance(audio);
  audio->start();
  */

  GameSelectScreen *select_screen = new GameSelectScreen();
  state->switch_to_screen(select_screen);

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

  delete select_screen;

  //delete audio;
  delete renderer;
  delete state;
  delete input;

  return 0;
}
