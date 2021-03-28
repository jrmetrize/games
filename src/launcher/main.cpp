#include <string>
#include <chrono>

#include "core/audio.h"
#include "core/graphics.h"
#include "core/input.h"
#include "core/state.h"

void
error_dialog(std::string error_message);

#ifdef _WIN32
#include <Windows.h>
#include <Winuser.h>

void
error_dialog(std::string error_message)
{
  MessageBox(nullptr, error_message.c_str(), "Error", MB_OK);
}
#endif

#ifdef __linux__
void
error_dialog(std::string error_message)
{

}
#endif

#ifdef __APPLE__
void
error_dialog(std::string error_message)
{

}
#endif

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

  AudioServer *audio = new AudioServer();
  AudioServer::set_instance(audio);
  audio->start();

  {
    AudioTrack *menu = (AudioTrack *)GameState::get()->get_globals()->get_resource("menu_music");
    AudioServer::get()->set_music(menu);
    AudioServer::get()->start_music();
  }

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

  delete audio;
  delete renderer;
  delete state;
  delete input;

  return 0;
}
